#include "./header.h"

int main(int argc, char *argv[])
{
  Status status;

  Version version = OpenNI::getVersion();
  printf("OpenNI version: %d.%d.%d, build: %d\n",
         version.major, version.minor, version.maintenance, version.build);

  if (status = error(OpenNI::initialize()))
    return status;

  Array<DeviceInfo> deviceInfoList;
  OpenNI::enumerateDevices(&deviceInfoList);
  for (int i = 0; i < deviceInfoList.getSize(); i++)
  {
    DeviceInfo deviceInfo = deviceInfoList[i];
    printf("Device: %s %s, URI: %s, VID: %d, PID: %d, SN: %s\n",
           deviceInfo.getVendor(), deviceInfo.getName(), deviceInfo.getUri(),
           deviceInfo.getUsbVendorId(), deviceInfo.getUsbProductId(),
           deviceInfo.getSerialNumber());
  }

  OpenNI::shutdown();

  return 0;
}
