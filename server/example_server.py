from concurrent import futures
import logging

import grpc

import object_detection_pb2
import object_detection_pb2_grpc


class ObjectDetectionServicer(object_detection_pb2_grpc.ObjectDetectionServicer):
    def __init__(self):
        pass

    def DetectObjects(self, request, context):
        print(f"Request received containing num points: {len(request.points)}")

        # TODO: implement detector here and add detections to output
        objects = object_detection_pb2.Objects()
        objects.objects.append(
            object_detection_pb2.Object(
                x=2.0,
                y=2.0,
                z=2.0,
                roll=2.0,
                pitch=2.0,
                yaw=2.0,
                l=2.0,
                w=2.0,
                h=2.0,
                confidence=1.0,
                classification="Vehicle",
            )
        )

        return objects


def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=1))
    object_detection_pb2_grpc.add_ObjectDetectionServicer_to_server(
        ObjectDetectionServicer(), server
    )
    server.add_insecure_port("[::]:50051")
    server.start()
    server.wait_for_termination()


if __name__ == "__main__":
    logging.basicConfig()
    serve()
