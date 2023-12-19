#include <iostream>
#include "OpenNI.h"

using namespace std;
using namespace openni;

int main(int argc, char *argv[])
{
  Array<DeviceInfo> deviceInfoList;

  OpenNI::initialize();
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
