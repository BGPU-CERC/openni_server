#include "./header.h"

Status error(Status error)
{
    if (error)
        printf("Error %d: %s", error, OpenNI::getExtendedError());
    return error;
}
