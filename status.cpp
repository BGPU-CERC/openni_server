#include "./header.h"

Status check(Status error)
{
    if (error)
    {
        throw exception(OpenNI::getExtendedError());
    }
    else
    {
        return error;
    }
}
