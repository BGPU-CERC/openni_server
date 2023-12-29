#include "./header.h"

void stream(VideoStream &depthStream)
{
#ifdef WIN32
    WSADATA wsaData;
    wsa_check(WSAStartup(MAKEWORD(2, 2), &wsaData));
#endif

    const struct sockaddr_in addr = {AF_INET, htons(8081), INADDR_ANY};
    SOCKET server_s = socket(AF_INET, SOCK_STREAM, 0);
    wsa_check(bind(server_s, (const sockaddr *)&addr, sizeof(addr)));
    wsa_check(listen(server_s, 10));

    char request[256] = {0};
    SOCKET client_s = accept(server_s, NULL, NULL);
    recv(client_s, request, sizeof(request), 0);
    cout << request << endl;

    VideoFrameRef frame;
    DepthPixel *depthPixel;

    char response[128];
    int bytesSent = 0;
    do
    {
        check(depthStream.readFrame(&frame));
        depthPixel = (DepthPixel *)frame.getData();

        cout
            << "index: " << frame.getFrameIndex()
            << ", timestamp: " << frame.getTimestamp()
            << ", w: " << frame.getWidth()
            << ", h: " << frame.getHeight()
            << "\n";

        float x;
        float y;
        float z;
        // float z = *(depthPixel + (v * frame.getHeight() + u));

        float world_x;
        float world_y;
        float world_z;

        for (int v = 0; v < frame.getHeight(); v++)
        {
            for (int u = 0; u < frame.getWidth(); u++)
            {
                x = (float)u;
                y = (float)v;
                z = *(depthPixel + (v * frame.getHeight() + u));
                // z = 0;

                // cout << *world_x << *world_y << *world_z << endl;

                check(CoordinateConverter::convertDepthToWorld(depthStream, x, y, z, &world_x, &world_y, &world_z));

                // bytesSent = send(client_s, "a", sizeof("a"), 0);

                // bytesSent = sprintf(response, "%f %f %f 255 255 255\n", *world_x, *world_y, *world_z);
                // cout << response << endl;
                // bytesSent = send(client_s, response, bytesSent, 0);

                // bytesSent = sprintf(response, "%f %f %f 255 255 255\n", world_x, world_y, world_z);
                bytesSent = sprintf(response, "%f 255 255 255\n", world_z);
                bytesSent = send(client_s, response, bytesSent, 0);

                if (!bytesSent)
                {
                    goto streamEnd;
                }
            }
        }
    } while (bytesSent > 0);

streamEnd:

    _close(client_s);
    _close(server_s);
}
