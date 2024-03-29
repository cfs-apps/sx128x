/*
    This file is part of GPIO++.
    Copyright (C) 2020 ReimuNotMoe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "GPIO++.hpp"
//cfs error: ‘unique_lock’ is not a member of ‘std’ note: ‘std::unique_lock’ is defined in header ‘<mutex>’; did you forget to ‘#include <mutex>’?
#include <mutex> //cfs

using namespace YukiWorkshop;

std::vector<GPIO::Device> GPIO::all_devices() {
	std::vector<GPIO::Device> ret;

	for (uint32_t i=0; i<UINT32_MAX; i++) {
		GPIO::Device d;
		try {
			d.open(i);
			ret.emplace_back(std::move(d));
		} catch (...) {
			break;
		}
	}

	return ret;
}

GPIO::Device GPIO::find_device_by_name(const std::string& __name) {
	auto devs = all_devices();

	for (auto &it : devs) {
		if (it.name() == __name)
			return it;
	}

	throw std::logic_error("no device with this name");
}

GPIO::Device GPIO::find_device_by_label(const std::string& __label) {
	auto devs = all_devices();

	for (auto &it : devs) {
		if (it.label() == __label)
			return it;
	}

	throw std::logic_error("no device with this label");
}

void GPIO::Device::get_device_info() {
	gpiochip_info cinfo;

	if (ioctl(fd, GPIO_GET_CHIPINFO_IOCTL, &cinfo)) {
		throw ExceptionWithErrno("failed to get device info");
	}

	name_ = cinfo.name;
	label_ = cinfo.label;
	num_lines_ = cinfo.lines;
}

std::map<uint32_t, std::string> &GPIO::Device::lines_by_num() {
	if (lines_by_num_.empty()) {
		for (uint32_t i=0; i<num_lines_; i++) {
			gpioline_info linfo{};
			linfo.line_offset = i;

			if (ioctl(fd, GPIO_GET_LINEINFO_IOCTL, &linfo))
				throw ExceptionWithErrno("failed to get line info");

			lines_by_num_.insert({i, linfo.name});
		}
	}

	return lines_by_num_;
}

std::map<std::string, uint32_t> &GPIO::Device::lines_by_name() {
	if (lines_by_name_.empty()) {
		for (uint32_t i=0; i<num_lines_; i++) {
			gpioline_info linfo{};
			linfo.line_offset = i;

			if (ioctl(fd, GPIO_GET_LINEINFO_IOCTL, &linfo))
				throw ExceptionWithErrno("failed to get line info");

			lines_by_name_.insert({linfo.name, i});
		}
	}

	return lines_by_name_;
}

void GPIO::Device::open(const std::string &__path) {
	if ((fd = ::open(__path.c_str(), O_RDWR)) == -1)
		throw ExceptionWithErrno("failed to open device");

	get_device_info();
	path_ = __path;

	if (debug)
		std::cerr << "GPIO++: " << "Device " << __path << " opened";
}

void GPIO::Device::open(uint32_t __id) {
	open(Utils::make_device_path(__id));
}

GPIO::LineSingle
GPIO::Device::line(uint32_t __line_number, GPIO::LineMode __mode, uint8_t __default_value, const std::string &__label) {
	gpiohandle_request req{};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,4,0)
	if (!((__mode & LineMode::PullUp) == LineMode::PullUp || (__mode & LineMode::PullDown) == LineMode::PullDown)) {
		__mode |= LineMode::NoPull;
	}
#endif

	req.lineoffsets[0] = __line_number;
	req.default_values[0] = __default_value;
	req.flags = (uint32_t)__mode;
	strncpy(req.consumer_label, __label.c_str(), 31);
	req.lines = 1;

	if (ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req))
		throw ExceptionWithErrno("failed to get line handle");

	gpioline_info linfo{};
	linfo.line_offset = __line_number;

	if (ioctl(fd, GPIO_GET_LINEINFO_IOCTL, &linfo))
		throw ExceptionWithErrno("failed to get line info");

	if (debug)
		std::cerr << "GPIO++: " << "Line " << __line_number << " opened, mode="
			  << (uint)__mode << ", default_value=" << __default_value << ", label=" << __label << "\n";

	return LineSingle(req.fd, fd, 1, linfo);
}

GPIO::LineMultiple
GPIO::Device::line(const std::initializer_list<LineSpec> &__lss, GPIO::LineMode __mode, const std::string &__label) {
	gpiohandle_request req{};

	uint8_t usable_size = __lss.size() > 64 ? 64 : __lss.size();

	for (uint8_t i=0; i<usable_size; i++) {
		req.lineoffsets[i] = (__lss.begin()+i)->line_number;
		req.default_values[i] = (__lss.begin()+i)->default_value;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,4,0)
	if (!((__mode & LineMode::PullUp) == LineMode::PullUp || (__mode & LineMode::PullDown) == LineMode::PullDown)) {
		__mode |= LineMode::NoPull;
	}
#endif

	strncpy(req.consumer_label, __label.c_str(), 31);
	req.flags = (uint32_t)__mode;
	req.lines = usable_size;

	if (ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req))
		throw ExceptionWithErrno("failed to get line handle");

	if (debug)
		for (uint8_t i=0; i<usable_size; i++) {
			std::cerr << "GPIO++: " << "Line(M) " << (__lss.begin()+i)->line_number << " opened, mode="
				  << (uint)__mode << ", default_value=" << (__lss.begin()+i)->default_value << "\n";
		}


	return LineMultiple(req.fd, usable_size);
}

int GPIO::Device::add_event(uint32_t __line_number, GPIO::LineMode __line_mode, GPIO::EventMode __event_mode,
			    const std::function<void(EventType, uint64_t)>& __handler, const std::string &__label) {
	std::unique_lock<std::shared_mutex> lk(event_lock);

	gpioevent_request req{};
	req.lineoffset = __line_number;
	req.handleflags = (uint32_t)__line_mode;
	req.eventflags = (uint32_t)__event_mode;
	strncpy(req.consumer_label, __label.c_str(), 31);

	if (ioctl(fd, GPIO_GET_LINEEVENT_IOCTL, &req)) {
		throw ExceptionWithErrno("failed to setup events");
	}

	events_map.insert({req.fd, __handler});

	if (epfd > 0) {
		epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.fd = req.fd;

		epoll_ctl(epfd, EPOLL_CTL_ADD, req.fd, &ev);
	}

	return req.fd;
}

void GPIO::Device::remove_event(int __event_handle) {
	std::unique_lock<std::shared_mutex> lk(event_lock);

	if (epfd > 0)
		epoll_ctl(epfd, EPOLL_CTL_DEL, __event_handle, nullptr);

	events_map.erase(__event_handle);
}

void GPIO::Device::process_event(int __event_handle) {
	std::shared_lock<std::shared_mutex> lk(event_lock);

	gpioevent_data event;
	auto it = events_map.find(__event_handle);
	if (it != events_map.end() &&
	    read(__event_handle, &event, sizeof(gpioevent_data)) == sizeof(gpioevent_data))
		events_map[__event_handle]((EventType)event.id, event.timestamp);
	else
		throw std::logic_error("event handle not found, check your code!");
}

std::vector<int> GPIO::Device::event_fds() {
	std::shared_lock<std::shared_mutex> lk(event_lock);

	std::vector<int> ret;
	for (auto &it : events_map) {
		ret.emplace_back(it.first);
	}
	return ret;
}

bool GPIO::Device::is_event_fd(int __fd) {
	std::shared_lock<std::shared_mutex> lk(event_lock);

	return events_map.find(__fd) != events_map.end();
}

void GPIO::Device::run_eventlistener() {
	eventlistener_run = true;

	epfd = epoll_create(42);

	std::shared_lock<std::shared_mutex> lk(event_lock);
	for (auto &it : events_map) {
		epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.fd = it.first;

		epoll_ctl(epfd, EPOLL_CTL_ADD, it.first, &ev);
	}
	lk.unlock();

	int ep_rc;
	epoll_event evs[16];

	while ((ep_rc = epoll_wait(epfd, evs, 16, 1000)) != -1) {
		if (ep_rc > 0) {
         //cfs error: comparison of integer expressions of different signedness: ‘uint’ {aka ‘unsigned int’} and ‘int’
			//cfs for (uint i=0; i<ep_rc; i++)
         for (int i=0; i<ep_rc; i++) //cfs
				process_event(evs[i].data.fd);
		}

		if (!eventlistener_run) {
			close(epfd);
			epfd = -1;
			break;
		}
	}

}

void GPIO::Device::stop_eventlistener() {
	eventlistener_run = false;
}

uint8_t GPIO::LineSingle::read() {
	gpiohandle_data data{};

	if (ioctl(fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data))
		throw ExceptionWithErrno("failed to read value from line");

	if (debug)
		std::cerr << "GPIO++: " << "Line " << number() << " '" << label() << "': value read: " << +data.values[0] << "\n";

	return data.values[0];
}

void GPIO::LineSingle::write(uint8_t __value) {
	gpiohandle_data data{};
	data.values[0] = __value;

	if (ioctl(fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data))
		throw ExceptionWithErrno("failed to write value to line");

	if (debug)
		std::cerr << "GPIO++: " << "Line " << number() << " '" << label() << "': value write: " << +data.values[0] << "\n";
}

GPIO::LineMode GPIO::LineSingle::mode() const {
	gpioline_info linfo{};
	linfo.line_offset = offset_;

	if (ioctl(pfd, GPIO_GET_LINEINFO_IOCTL, &linfo))
		throw ExceptionWithErrno("failed to get line info");

	return (LineMode)linfo.flags;
}

void GPIO::LineSingle::set_mode(GPIO::LineMode __mode, uint8_t __default_value, const std::string &__label) {
	close(fd);

	gpiohandle_request req{};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,4,0)
	if (!((__mode & LineMode::PullUp) == LineMode::PullUp || (__mode & LineMode::PullDown) == LineMode::PullDown)) {
		__mode |= LineMode::NoPull;
	}
#endif

	req.lineoffsets[0] = offset_;
	req.default_values[0] = __default_value;
	req.flags = (uint32_t)__mode;
	strncpy(req.consumer_label, __label.c_str(), 31);
	req.lines = 1;

	if (ioctl(pfd, GPIO_GET_LINEHANDLE_IOCTL, &req))
		throw ExceptionWithErrno("failed to get line handle");

	if (debug)
		std::cerr << "GPIO++: " << "Line " << number() << ": mode changed, mode="
			  << (uint)__mode << ", default_value=" << __default_value << ", label=" << __label << "\n";

	fd = req.fd;
}

std::vector<uint8_t> GPIO::LineMultiple::read() {
	std::vector<uint8_t> ret(sizeof(gpiohandle_data));

	if (ioctl(fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, ret.data()))
		throw ExceptionWithErrno("failed to read values from lines");

	ret.resize(size);
	return ret;
}

void GPIO::LineMultiple::write(const std::vector<uint8_t> &__values) {
	if (ioctl(fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, __values.data()))
		throw ExceptionWithErrno("failed to write values to lines");
}
