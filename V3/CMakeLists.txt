aux_source_directory(./server/src/ SERVER_SRC_LIST)
aux_source_directory(./client/src/ CLIENT_SRC_LIST)
include_directories(./server/include/ ./client/include)

add_executable(V3-server ${SERVER_SRC_LIST})
add_executable(V3-client ${CLIENT_SRC_LIST})

target_compile_options(V3-server PRIVATE -Wall -g -O0)
target_compile_options(V3-client PRIVATE -Wall -g -O0)
