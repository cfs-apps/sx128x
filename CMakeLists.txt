project(CFS_SX128X CXX)

include_directories(fsw/src)

aux_source_directory(fsw/src LIB_SRC_FILES)

# Create the app module
add_cfe_app(sx128x ${LIB_SRC_FILES})

