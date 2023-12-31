#include "./header.h"

std::stringstream status_ss;
std::string status_str;

Status check(Status error)
{
    if (error)
    {
        status_ss.str(std::string());
        status_ss.clear();
        status_ss
            << "(" << error << ") "
            << OpenNI::getExtendedError();

        status_str = status_ss.str();
        throw exception(status_str.c_str());
    }
    else
    {
        return error;
    }
}

int wsa_check(int status)
{
    if (status)
    {
        status_ss.str(std::string());
        status_ss.clear();
        status_ss
            << "(" << status << ") "
            << "WSA error: " << WSAGetLastError();

        status_str = status_ss.str();
        throw exception(status_str.c_str());
    }
    else
    {
        return status;
    }
}
