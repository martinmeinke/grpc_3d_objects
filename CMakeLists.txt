# Minimum CMake required
cmake_minimum_required(VERSION 3.15)

# Project
project(object_detection)

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
get_filename_component(hw_proto "${CMAKE_CURRENT_LIST_DIR}/protocol/object_detection.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/object_detection.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/object_detection.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/object_detection.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/object_detection.grpc.pb.h")
add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

message(WARNING ${hw_proto_srcs})
message(WARNING ${hw_grpc_srcs})
message(WARNING ${CMAKE_CURRENT_BINARY_DIR})

add_library(grpc_3d_objects_library "client/object_detection_client.cpp" ${hw_proto_srcs} ${hw_grpc_srcs})

target_link_libraries(grpc_3d_objects_library
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

add_executable(grpc_3d_objects_client "client/client.cpp")

target_link_libraries(grpc_3d_objects_client
    grpc_3d_objects_library)