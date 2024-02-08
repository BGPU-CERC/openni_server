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

void stream(astra::StreamReader &reader, astra::DepthStream &depthStream)
{
    char request[128] = {0};
    client_s = accept(server_s, NULL, NULL);

    int frameWidth = 640;
    int frameHeight = 480;

    const auto coordinateMapper = depthStream.coordinateMapper();
    astra::Vector3f depthPoint;
    astra::Vector3f worldPoint;

    float *response = new float[1000 * 1000];
    int response_index = 0;
    int response_len = 0;
    int bytesSent = 0;

    do
    {
        recv(client_s, request, sizeof(request), 0);

        astra::Frame frame = reader.get_latest_frame();
        const auto depthFrame = frame.get<astra::DepthFrame>();
        const auto depthFrameData = depthFrame.data();

        cout << endl
             << "frame: " << depthFrame.frame_index();

        for (int v = 0; v < frameHeight; v += 5)
        {
            for (int u = 0; u < frameWidth; u += 5)
            {
                depthPoint.x = (float)u;
                depthPoint.y = (float)v;
                depthPoint.z = (float)depthFrameData[v * frameWidth + u];

                worldPoint = coordinateMapper.convert_depth_to_world(depthPoint);
                response[response_index++] = worldPoint.x;
                response[response_index++] = worldPoint.y;
                response[response_index++] = worldPoint.z;
            }
        }

        response_len = response_index * sizeof(float);
        bytesSent = send(client_s, (char *)&response_len, sizeof(response_len), 0);
        bytesSent = send(client_s, (char *)response, response_len, 0);
        cout << " vertex count: " << response_index / 3
             << " bytes sent: " << bytesSent << endl;

        response_index = 0;
    } while (bytesSent != -1);

    wsa_check(closesocket(client_s));
    delete[] response;
}
