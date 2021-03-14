# Introduction
grcp_3d_objects is a GRCP client/server stub for executing Python-based 3d bounding box detection on pointclouds from a C++ context.

# Use Case
You want to execute some python-based LiDAR object detection code from C++, but don't want to fiddle around with PyBind, virtualenvs etc.

Thir repo implements a Python based server that receives PointClouds from a C++ client and returns Detected Objects.

Communication is handled via gRPC, interfaces are defined via protobuf.

# HowTo
1. Install grcp:

    https://grpc.io/docs/languages/cpp/quickstart/

2. Build client lib & code using cmake 
   
   `mkdir build && cd build && cmake .. && make`

3. Generate python grpc stub

   `./gen_proto.sh`

4. Run server & client
   
   `./build/grpc_3d_objects_client`

   `./server/example_server.py`