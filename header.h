#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "OpenNI.h"

using namespace std;
using namespace openni;

Status check(Status error);

void print_version();

const char *device_to_string(DeviceInfo &deviceInfo);
void print_device_list();
