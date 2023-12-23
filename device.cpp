#include "./header.h"

std::stringstream ss;
std::string str;

const char *device_to_string(DeviceInfo &deviceInfo)
{
    ss << "Device: " << deviceInfo.getVendor() << " " << deviceInfo.getName()
       << ", URI: " << deviceInfo.getUri()
       << ", VID: " << deviceInfo.getUsbVendorId()
       << ", PID: " << deviceInfo.getUsbProductId()
       << ", SN: " << deviceInfo.getSerialNumber();

    str = ss.str();
    return str.c_str();
}

void print_device_list()
{
    Array<DeviceInfo> deviceInfoList;
    OpenNI::enumerateDevices(&deviceInfoList);
    for (int i = 0; i < deviceInfoList.getSize(); i++)
    {
        DeviceInfo deviceInfo = deviceInfoList[i];
        printf("%s\n", device_to_string(deviceInfo));
    }
}
