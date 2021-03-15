#include <iostream>
#include <string>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "object_detection.grpc.pb.h"

using grpc::ClientContext;
using grpc::Status;

class ObjectDetectionClient {
public:
  ObjectDetectionClient(std::string server_address)
      : stub_(ObjectDetection::NewStub(grpc::CreateChannel(
            server_address, grpc::InsecureChannelCredentials()))) {}

  Objects DetectObjects(Points &request) {
    Objects reply;
    ClientContext context;

    // Here we can use the stub's newly available method we just added.
    Status status = stub_->DetectObjects(&context, request, &reply);
    if (status.ok()) {
      return reply;
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return Objects();
    }
  }

private:
  std::unique_ptr<ObjectDetection::Stub> stub_;
};
