#include "./header.h"

std::stringstream device_ss;
std::string device_str;

const char *device_to_string(const DeviceInfo &deviceInfo)
{
    device_ss.str(std::string());
    device_ss.clear();
    device_ss
        << "Device: " << deviceInfo.getVendor() << " " << deviceInfo.getName()
        << ", URI: " << deviceInfo.getUri()
        << ", VID: " << deviceInfo.getUsbVendorId()
        << ", PID: " << deviceInfo.getUsbProductId()
        << ", SN: " << deviceInfo.getSerialNumber();

    device_str = device_ss.str();
    return device_str.c_str();
}

void print_device_list()
{
    Array<DeviceInfo> deviceInfoList;
    OpenNI::enumerateDevices(&deviceInfoList);
    for (int i = 0; i < deviceInfoList.getSize(); i++)
    {
        const DeviceInfo &deviceInfo = deviceInfoList[i];
        printf("%s\n", device_to_string(deviceInfo));
    }
}

Status device_init_registration(Device &device)
{
    if (device.isImageRegistrationModeSupported(IMAGE_REGISTRATION_DEPTH_TO_COLOR))
    {
        cout << "IMAGE_REGISTRATION_DEPTH_TO_COLOR is supported, enabling\n";
        return device.setImageRegistrationMode(IMAGE_REGISTRATION_DEPTH_TO_COLOR);
    }
    else
    {
        cout << "IMAGE_REGISTRATION_DEPTH_TO_COLOR is not supported, skipping\n";
        return STATUS_OK;
    }
}
