#include "include/object_detection_client.h"

int main(int argc, char **argv)
{
    Points request;
    Point *sample_point = request.add_points();
    sample_point->set_x(1.0);
    sample_point->set_y(1.0);
    sample_point->set_z(1.0);
    sample_point->set_intensity(1.0);

    std::string server_address("localhost:50051");
    ObjectDetectionClient odc(server_address);
    std::string reply = odc.DetectObjects(request);
    std::cout << "Response: " << reply << std::endl;

    return 0;
}