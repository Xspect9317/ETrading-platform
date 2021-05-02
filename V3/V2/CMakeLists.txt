aux_source_directory(./src/ SRC_LIST)
include_directories(./include/)

add_executable(V2 ${SRC_LIST})

target_compile_options(V3 PRIVATE -Wall -g -O0)
