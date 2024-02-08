#include "./header.h"

std::stringstream status_ss;
std::string status_str;

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

astra_status_t astra_check(astra_status_t status)
{
    if (status)
    {
        status_ss.str(std::string());
        status_ss.clear();
        status_ss << "(" << status << ") ";

        switch (status)
        {
        case ASTRA_STATUS_INVALID_PARAMETER:
            status_ss << "ASTRA_STATUS_INVALID_PARAMETER";
            break;
        case ASTRA_STATUS_DEVICE_ERROR:
            status_ss << "ASTRA_STATUS_DEVICE_ERROR";
            break;
        case ASTRA_STATUS_TIMEOUT:
            status_ss << "ASTRA_STATUS_TIMEOUT";
            break;
        case ASTRA_STATUS_INVALID_PARAMETER_TOKEN:
            status_ss << "ASTRA_STATUS_INVALID_PARAMETER_TOKEN";
            break;
        case ASTRA_STATUS_INVALID_OPERATION:
            status_ss << "ASTRA_STATUS_INVALID_OPERATION";
            break;
        case ASTRA_STATUS_INTERNAL_ERROR:
            status_ss << "ASTRA_STATUS_INTERNAL_ERROR";
            break;
        case ASTRA_STATUS_UNINITIALIZED:
            status_ss << "ASTRA_STATUS_UNINITIALIZED";
            break;
        }

        status_str = status_ss.str();
        throw exception(status_str.c_str());
    }
    else
    {
        return status;
    }
}
