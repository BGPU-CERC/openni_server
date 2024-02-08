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

void stream(astra::StreamReader &reader, astra::DepthStream &depthStream, astra::ColorStream &colorStream)
{
    char request[128] = {0};
    client_s = accept(server_s, NULL, NULL);

    int frameWidth = 640;
    int frameHeight = 480;

    const auto coordinateMapper = depthStream.coordinateMapper();
    astra::Vector3f depthPoint;
    astra::Vector3f worldPoint;
    astra::RgbPixel rgbPixel;

    float *coords = new float[500 * 1000];
    uint32_t coordsByteLength = 0;

    uint8_t *colors = new uint8_t[500 * 1000];
    uint32_t colorsByteLength = 0;

    int point_index = 0;
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

        cout << endl
             << "frame: " << depthFrame.frame_index();

        for (int v = 0; v < frameHeight; v += 3)
        {
            for (int u = 0; u < frameWidth; u += 3)
            {
                depthPoint.x = (float)u;
                depthPoint.y = (float)v;
                depthPoint.z = (float)depthFrameData[v * frameWidth + u];

                worldPoint = coordinateMapper.convert_depth_to_world(depthPoint);
                rgbPixel = colorFrameData[v * frameWidth + u];

                coords[point_index] = worldPoint.x;
                colors[point_index++] = rgbPixel.r;

                coords[point_index] = worldPoint.y;
                colors[point_index++] = rgbPixel.g;

                coords[point_index] = worldPoint.z;
                colors[point_index++] = rgbPixel.b;
            }
        }

        coordsByteLength = point_index * sizeof(float);
        colorsByteLength = point_index * sizeof(uint8_t);
        response_len = coordsByteLength + colorsByteLength;

        bytesSent = send(client_s, (char *)&response_len, sizeof(response_len), 0);
        bytesSent = send(client_s, (char *)coords, coordsByteLength, 0);
        bytesSent = send(client_s, (char *)colors, colorsByteLength, 0);
        cout << " vertex count: " << point_index / 3
             << " byte length: " << response_len << endl;

        point_index = 0;
    } while (bytesSent != -1);

    wsa_check(closesocket(client_s));
    delete[] coords;
    delete[] colors;
}
