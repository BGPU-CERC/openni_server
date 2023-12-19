#include <iostream>
#include "OpenNI.h"

using namespace std;
using namespace openni;

Status error(Status error)
{
  if (error)
    printf("Error %d: %s", error, OpenNI::getExtendedError());
  return error;
}

int main(int argc, char *argv[])
{
  Array<DeviceInfo> deviceInfoList;
  Status status;

  if (status = error(OpenNI::initialize()))
    return status;

  OpenNI::enumerateDevices(&deviceInfoList);

  for (int i = 0; i < deviceInfoList.getSize(); i++)
  {
    DeviceInfo deviceInfo = deviceInfoList[i];
    printf("Device: %s %s, URI: %s, VID: %d, PID: %d, SN: %s",
           deviceInfo.getVendor(), deviceInfo.getName(), deviceInfo.getUri(),
           deviceInfo.getUsbVendorId(), deviceInfo.getUsbProductId(),
           deviceInfo.getSerialNumber());
  }

  OpenNI::shutdown();

  return 0;
}
