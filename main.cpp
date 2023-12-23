#include "./header.h"

int main(int argc, char *argv[])
{
  try
  {
    print_version();
    check(OpenNI::initialize());
    print_device_list();

    Device device;
    check(device.open(ANY_DEVICE));
    check(device.setImageRegistrationMode(IMAGE_REGISTRATION_DEPTH_TO_COLOR));
    check(device.setDepthColorSyncEnabled(true));

    const SensorInfo *sensorInfo = device.getSensorInfo(SENSOR_DEPTH);

    VideoStream videoStream;
    check(videoStream.create(device, SENSOR_DEPTH));
    check(videoStream.start());

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
