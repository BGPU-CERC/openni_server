#include "./header.h"

std::stringstream videomode_ss;
std::string videomode_str;

std::string pixel_format_str;
const char *pixel_format_to_string(PixelFormat pixel_format)
{
    switch (pixel_format)
    {
    case ONI_PIXEL_FORMAT_DEPTH_1_MM:
        pixel_format_str = "ONI_PIXEL_FORMAT_DEPTH_1_MM";
        break;
    case ONI_PIXEL_FORMAT_DEPTH_100_UM:
        pixel_format_str = "ONI_PIXEL_FORMAT_DEPTH_100_UM";
        break;
    case ONI_PIXEL_FORMAT_SHIFT_9_2:
        pixel_format_str = "ONI_PIXEL_FORMAT_SHIFT_9_2";
        break;
    case ONI_PIXEL_FORMAT_SHIFT_9_3:
        pixel_format_str = "ONI_PIXEL_FORMAT_SHIFT_9_3";
        break;
    case ONI_PIXEL_FORMAT_RGB888:
        pixel_format_str = "ONI_PIXEL_FORMAT_RGB888";
        break;
    case ONI_PIXEL_FORMAT_YUV422:
        pixel_format_str = "ONI_PIXEL_FORMAT_YUV422";
        break;
    case ONI_PIXEL_FORMAT_GRAY8:
        pixel_format_str = "ONI_PIXEL_FORMAT_GRAY8";
        break;
    case ONI_PIXEL_FORMAT_GRAY16:
        pixel_format_str = "ONI_PIXEL_FORMAT_GRAY16";
        break;
    case ONI_PIXEL_FORMAT_JPEG:
        pixel_format_str = "ONI_PIXEL_FORMAT_JPEG";
        break;
    case ONI_PIXEL_FORMAT_YUYV:
        pixel_format_str = "ONI_PIXEL_FORMAT_YUYV";
        break;
    case ONI_PIXEL_FORMAT_LOG:
        pixel_format_str = "ONI_PIXEL_FORMAT_LOG";
        break;
    default:
        pixel_format_str = "UNKNOWN_FORMAT";
        break;
    }

    return pixel_format_str.c_str();
}

const char *videomode_to_string(const VideoMode &m)
{
    videomode_ss.str(std::string());
    videomode_ss.clear();
    videomode_ss
        << "FPS: " << m.getFps()
        << ", Resolution: " << m.getResolutionX() << "x" << m.getResolutionY()
        << ", PixelFormat: " << pixel_format_to_string(m.getPixelFormat());

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