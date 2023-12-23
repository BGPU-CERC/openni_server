#include "./header.h"

std::stringstream videomode_ss;
std::string videomode_str;

const char *videomode_to_string(const VideoMode &m)
{
    videomode_ss.str(std::string());
    videomode_ss.clear();
    videomode_ss
        << "FPS: " << m.getFps()
        << ", Resolution: " << m.getResolutionX() << "x" << m.getResolutionY()
        << ", PixelFormat: " << m.getPixelFormat();

    videomode_str = videomode_ss.str();
    return videomode_str.c_str();
}

void sensor_print_videomodes(const Array<VideoMode> &videoModes)
{
    for (int i = 0; i < videoModes.getSize(); i++)
    {
        const VideoMode &videoMode = videoModes[i];
        printf("%s\n", videomode_to_string(videoMode));
    }
}