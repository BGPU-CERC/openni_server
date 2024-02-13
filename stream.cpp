#include "./header.h"

SOCKET server_s;
SOCKET client_s;

void stream_server_start()
{
    WSADATA wsaData;
    wsa_check(WSAStartup(MAKEWORD(2, 2), &wsaData));

    const struct sockaddr_in addr = {AF_INET, htons(8081), INADDR_ANY};
    server_s = socket(AF_INET, SOCK_STREAM, 0);
    wsa_check(bind(server_s, (const sockaddr *)&addr, sizeof(addr)));
    wsa_check(listen(server_s, 10));
}

void stream(astra::StreamReader &reader, astra::DepthStream &depthStream, astra::ColorStream &colorStream, astra::BodyStream &bodyStream)
{
    char request[128] = {0};
    client_s = accept(server_s, NULL, NULL);

    int frameWidth = depthStream.mode().width();
    int frameHeight = depthStream.mode().height();
    int depth_min = 20;

    const auto coordinateMapper = depthStream.coordinateMapper();
    astra::Vector3f depthPoint;
    astra::Vector3f worldPoint;
    astra::RgbPixel rgbPixel;

    float *pointCoords = new float[500 * 1000];
    uint32_t pointCoordsByteLength = 0;

    uint8_t *pointColors = new uint8_t[500 * 1000];
    uint32_t pointColorsByteLength = 0;

    astra::Vector3f jointCoords[9];
    astra::HandPose handPoses[2];

    int point_index = 0;
    int point_count = 0;
    int response_len = 0;
    int bytesSent = 0;

    do
    {
        recv(client_s, request, sizeof(request), 0);

        astra::Frame frame = reader.get_latest_frame();

        const auto depthFrame = frame.get<astra::DepthFrame>();
        const auto depthFrameData = depthFrame.data();

        const auto colorFrame = frame.get<astra::ColorFrame>();
        const auto colorFrameData = colorFrame.data();

        const auto bodyFrame = frame.get<astra::BodyFrame>();
        const auto body = bodyFrame.bodies().front();
        const auto bodyJoints = body.joints();

        for (int v = 0; v < frameHeight; v += 3)
        {
            for (int u = 0; u < frameWidth; u += 3)
            {
                depthPoint.x = (float)u;
                depthPoint.y = (float)v;
                depthPoint.z = (float)depthFrameData[v * frameWidth + u];

                if (depthPoint.z < depth_min)
                    continue;

                worldPoint = coordinateMapper.convert_depth_to_world(depthPoint);
                rgbPixel = colorFrameData[v * frameWidth + u];

                pointCoords[point_index] = worldPoint.x;
                pointColors[point_index++] = rgbPixel.r;

                pointCoords[point_index] = worldPoint.y;
                pointColors[point_index++] = rgbPixel.g;

                pointCoords[point_index] = worldPoint.z;
                pointColors[point_index++] = rgbPixel.b;
            }
        }

        for (int i = 0; i < 9; i++)
        {
            const auto bodyJoint = bodyJoints[i];
            jointCoords[i] = bodyJoint.world_position();
        }

        handPoses[0] = body.hand_poses().left_hand();
        handPoses[1] = body.hand_poses().right_hand();

        point_count = point_index / 3;
        pointCoordsByteLength = point_index * sizeof(float);
        pointColorsByteLength = point_index * sizeof(uint8_t);
        response_len = sizeof(point_count) + pointCoordsByteLength + pointColorsByteLength + sizeof(jointCoords) + sizeof(handPoses);
        bytesSent = send(client_s, (char *)&response_len, sizeof(response_len), 0);

        bytesSent = send(client_s, (char *)&point_count, sizeof(point_count), 0);
        bytesSent = send(client_s, (char *)pointCoords, pointCoordsByteLength, 0);
        bytesSent = send(client_s, (char *)pointColors, pointColorsByteLength, 0);
        bytesSent = send(client_s, (char *)jointCoords, sizeof(jointCoords), 0);
        bytesSent = send(client_s, (char *)handPoses, sizeof(handPoses), 0);

        cout << endl
             << "frame: " << depthFrame.frame_index()
             << " point_count: " << point_count
             << " response_length: " << response_len << endl;

        point_index = 0;
    } while (bytesSent != -1);

    wsa_check(closesocket(client_s));
    delete[] pointCoords;
    delete[] pointColors;
}
