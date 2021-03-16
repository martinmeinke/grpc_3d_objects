# Minimum CMake required
cmake_minimum_required(VERSION 3.15)

# Project
project(grpc_3d_objects VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

# Protobuf
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

# Protobuf-compiler
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# gRPC
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# Proto file
get_filename_component(proto "${CMAKE_CURRENT_LIST_DIR}/protocol/object_detection.proto" ABSOLUTE)
get_filename_component(proto_path "${proto}" PATH)

get_filename_component(generated_grpc_and_proto_dir "${CMAKE_CURRENT_BINARY_DIR}/include/grpc_3d_objects" ABSOLUTE)

# Generated sources
set(gen_proto_srcs "${generated_grpc_and_proto_dir}/object_detection.pb.cc")
set(gen_proto_hdrs "${generated_grpc_and_proto_dir}/object_detection.pb.h")
set(gen_grpc_srcs "${generated_grpc_and_proto_dir}/object_detection.grpc.pb.cc")
set(gen_grpc_hdrs "${generated_grpc_and_proto_dir}/object_detection.grpc.pb.h")
add_custom_command(
      OUTPUT "${gen_proto_srcs}" "${gen_proto_hdrs}" "${gen_grpc_srcs}" "${gen_grpc_hdrs}"
      COMMAND ${CMAKE_COMMAND} -E make_directory ${generated_grpc_and_proto_dir}
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${generated_grpc_and_proto_dir}"
        --cpp_out "${generated_grpc_and_proto_dir}"
        -I "${proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${proto}"
      DEPENDS "${proto}")

# Include generated *.pb.h files
# include_directories("${CMAKE_CURRENT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build shared (dynamic) libraries." ON)
add_library(${PROJECT_NAME}_library "client/object_detection_client.cpp" ${gen_proto_srcs} ${gen_grpc_srcs})
target_include_directories(${PROJECT_NAME}_library PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE client/include)

target_link_libraries(${PROJECT_NAME}_library
  PUBLIC
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

add_executable(${PROJECT_NAME}_example_client "client/example_client.cpp")
target_include_directories(${PROJECT_NAME}_example_client PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE client/include)

target_link_libraries(${PROJECT_NAME}_example_client
  PRIVATE
    ${PROJECT_NAME}_library)

install(TARGETS ${PROJECT_NAME}_library ${PROJECT_NAME}_example_client EXPORT ${PROJECT_NAME}-config
ARCHIVE DESTINATION lib
LIBRARY DESTINATION lib
RUNTIME DESTINATION bin)
install(DIRECTORY client/include/ DESTINATION include)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/ DESTINATION include)
install(EXPORT ${PROJECT_NAME}-config DESTINATION cmake)
export(TARGETS ${PROJECT_NAME}_library FILE ${PROJECT_NAME}-config.cmake)