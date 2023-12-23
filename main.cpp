#include "./header.h"

int main(int argc, char *argv[])
{
  try
  {
    print_version();
    check(OpenNI::initialize());

    cout << "Found devices:\n";
    print_device_list();

    Device device;
    check(device.open(ANY_DEVICE));
    check(device.setDepthColorSyncEnabled(true));

    const SensorInfo *sensorInfo = device.getSensorInfo(SENSOR_DEPTH);
    if (sensorInfo == NULL)
    {
      cout << "SENSOR_DEPTH is not available";
      return -1;
    }

    const Array<VideoMode> &videoModes = sensorInfo->getSupportedVideoModes();
    const VideoMode &videoMode = videoModes[4];
    cout << "Supported video modes:\n";
    sensor_print_videomodes(videoModes);
    cout << "Selected video mode:\n"
         << videomode_to_string(videoMode) << "\n";

    VideoStream videoStream;
    check(videoStream.create(device, SENSOR_DEPTH));
    check(videoStream.setVideoMode(videoMode));
    check(videoStream.start());

    check(device_init_registration(device));

    OpenNI::shutdown();
    return STATUS_OK;
  }
  catch (const std::exception &e)
  {
    printf("Exception: %s\n", e.what());
    OpenNI::shutdown();
    return -1;
  }
}
