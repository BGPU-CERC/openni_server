#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "OpenNI.h"

using namespace std;
using namespace openni;

Status check(Status error);

void print_version();

const char *device_to_string(const DeviceInfo &deviceInfo);
Status device_init_registration(Device &device);
void print_device_list();

const char *videomode_to_string(const VideoMode &m);
void sensor_print_videomodes(const Array<VideoMode> &videoModes);
