/*
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU Lesser General Public License as
**  published by the Free Software Foundation, either version 3 of the
**  License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Lesser General Public License for more details.
**
**  You should have received a copy of the GNU Lesser General Public License
**  along with this program.  If not, see <https://www.gnu.org/licenses/>.
**
**  Purpose:
**    Define radio interface class
**
**  Notes:
**    1. Serves as a bridge between the C++ Radio object and the
**       LoRa app. This header shouldn't include cFE headers or
**       ant C++ headers.
**    2. This can't include cFE or Basecamp headers
**
*/

#ifndef _radio_
#define _radio_

/*
** Includes
*/
#include <stdint.h>
#include <stdbool.h>

/***********************/
/** Macro Definitions **/
/***********************/


/**********************/
/** Type Definitions **/
/**********************/


typedef struct
{
   uint8_t Busy;
   uint8_t Nrst;
   uint8_t Nss;
   uint8_t Dio1;
   uint8_t Dio2;
   uint8_t Dio3;
   uint8_t TxEn;
   uint8_t RxEn;
      
} RADIO_Pin_t;


/************************/
/** Exported Functions **/
/************************/


/******************************************************************************
** Function: RADIO_Constructor
**
** Initialize the Radio object to a known state
**
** Notes:
**   1. This must be called prior to any other function.
**
*/
bool RADIO_Constructor(const char *SpiDevStr, uint8_t SpiDevNum, const RADIO_Pin_t *RadioPin);


/******************************************************************************
** Function: RADIO_SetLowNoiseAmpMode
**
** Set the radio power amp sensitivity
**
** Notes:
**   None
**
*/
bool RADIO_SetLowNoiseAmpMode(uint16_t LowNoiseAmpMode);


/******************************************************************************
** Function: RADIO_SetModulationParams
**
** Set the radio Lora Modulation parameters
**
** Notes:
**   None
**
*/
bool RADIO_SetModulationParams(uint8_t SpreadingFactor,
                               uint8_t Bandwidth,
                               uint8_t CodingRate);


/******************************************************************************
** Function: RADIO_SetRadioFrequency
**
** Set the radio frequency (Hz)
**
** Notes:
**   None
**
*/
bool RADIO_SetRadioFrequency(uint32_t Frequency);


/******************************************************************************
** Function: RADIO_SetPowerAmpRampTime
**
** Set the radio power amp ramp time
**
** Notes:
**   None
**
*/
bool RADIO_SetPowerAmpRampTime(uint16_t PowerAmpRampTime);


/******************************************************************************
** Function: RADIO_SetPowerRegulatorMode
**
** Set the radio power regulator mode
**
** Notes:
**   None
**
*/
bool RADIO_SetPowerRegulatorMode(uint16_t PowerRegulatorMode);


/******************************************************************************
** Function: RADIO_SetSpiSpeed
**
** Set the SPI speed
**
** Notes:
**   1. Not intended to be a ground command and assumes the Radio has been
**      initialized and speed value has been validated 
**
*/
bool RADIO_SetSpiSpeed(uint32_t SpiSpeed);


/******************************************************************************
** Function: RADIO_SetStandbyMode
**
** Set the radio standby mode
**
** Notes:
**   None
**
*/
bool RADIO_SetStandbyMode(uint16_t StandbyMode);


#endif /* _radio_ */
