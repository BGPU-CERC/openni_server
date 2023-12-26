#include "./header.h"

void StreamListener::onNewFrame(VideoStream &stream)
{
    stream.readFrame(&frame);

    cout
        << "index: " << frame.getFrameIndex()
        << ", timestamp: " << frame.getTimestamp()
        << ", w: " << frame.getWidth()
        << ", h: " << frame.getHeight()
        << "\n";

    DepthPixel *depthPixel = (DepthPixel *)frame.getData();
    cout << (DepthPixel *)frame.getData();

    for (int v = 0; v < frame.getHeight(); v++)
    {
        for (int u = 0; u < frame.getWidth(); u++)
        {
            float x = (float)u;
            float y = (float)v;
            float z = *(depthPixel + (v * frame.getHeight() + u));

            float *world_x = 0;
            float *world_y = 0;
            float *world_z = 0;

            try
            {
                check(CoordinateConverter::convertDepthToWorld(stream, x, y, z, world_x, world_y, world_z));
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }

            // fprintf(fp, "%f %f %f 255 255 255\n", *world_x, *world_y, *world_z);
        }
    }
}