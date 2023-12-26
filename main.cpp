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

    const SensorInfo *depthInfo = device.getSensorInfo(SENSOR_DEPTH);
    if (depthInfo == NULL)
    {
      cout << "SENSOR_DEPTH is not available\n";
      return -1;
    }

    const Array<VideoMode> &depthModes = depthInfo->getSupportedVideoModes();
    const VideoMode &depthMode = depthModes[4];
    cout << "Supported depth modes:\n";
    sensor_print_videomodes(depthModes);
    cout << "Selected depth mode:\n"
         << videomode_to_string(depthMode) << "\n";

    VideoStream depthStream;
    check(depthStream.create(device, SENSOR_DEPTH));
    check(depthStream.setVideoMode(depthMode));
    check(depthStream.setMirroringEnabled(true));
    check(depthStream.start());
    cout
        << "Min pixel value: " << depthStream.getMinPixelValue() << "\n"
        << "Max pixel value: " << depthStream.getMaxPixelValue() << "\n";

    check(device_init_registration(device));

    stream_server_start();
    while (true)
      stream(depthStream);
  }
  catch (const std::exception &e)
  {
    printf("Exception: %s\n", e.what());
    OpenNI::shutdown();
    return -1;
  }
}
