#include "./header.h"

void print_version()
{
    Version version = OpenNI::getVersion();
    printf("OpenNI version: %d.%d.%d, build: %d\n",
           version.major, version.minor, version.maintenance, version.build);
}