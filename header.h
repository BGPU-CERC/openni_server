#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <string>
#include <sstream>
#include <io.h>
#include "OpenNI.h"
#include <astra/astra.hpp>

using std::cout;
using std::endl;
using std::exception;
using namespace openni;

Status openni_check(Status error);
int wsa_check(int status);

void print_version();

const char *device_to_string(const DeviceInfo &deviceInfo);
Status device_init_registration(Device &device);
void print_device_list();

const char *videomode_to_string(const VideoMode &m);
void sensor_print_videomodes(const Array<VideoMode> &videoModes);

void stream_server_start();
void stream(VideoStream &depthStream);