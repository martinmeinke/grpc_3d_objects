#!/usr/bin/env sh

python -m grpc_tools.protoc -I protocol --python_out=server --grpc_python_out=server protocol/object_detection.proto
