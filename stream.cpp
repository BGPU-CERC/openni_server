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

void stream(VideoStream &depthStream)
{
    char request[128] = {0};
    client_s = accept(server_s, NULL, NULL);

    VideoFrameRef frame;
    int frameWidth = depthStream.getVideoMode().getResolutionX();
    int frameHeight = depthStream.getVideoMode().getResolutionY();

    DepthPixel *depthPixel;
    uint16_t min_depth = 20;
    uint16_t max_depth = 9000;
    float x;
    float y;
    float z;

    float response[100 * 1000];
    int response_index = 0;
    int response_len = 0;
    int bytesSent = 0;

    do
    {
        recv(client_s, request, sizeof(request), 0);

        check(depthStream.readFrame(&frame));
        depthPixel = (DepthPixel *)frame.getData();

        cout << endl
             << "frame: " << frame.getFrameIndex();

        for (int v = 0; v < frameHeight; v += 3)
        {
            for (int u = 0; u < frameWidth; u += 3)
            {
                x = (float)u;
                y = (float)v;
                z = depthPixel[v * frameWidth + u];

                if (z < min_depth || z > max_depth)
                    continue;

                check(CoordinateConverter::convertDepthToWorld(
                    depthStream, x, y, z,
                    (float *)response + response_index++,
                    (float *)response + response_index++,
                    (float *)response + response_index++));
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
}
