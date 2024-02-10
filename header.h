#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <string>
#include <sstream>
#include <io.h>
#include <astra/astra.hpp>

using std::cout;
using std::endl;
using std::exception;

astra_status_t astra_check(astra_status_t status);
int wsa_check(int status);

void stream_server_start();
void stream(astra::StreamReader &reader, astra::DepthStream &depthStream, astra::ColorStream &colorStream, astra::BodyStream &bodyStream);