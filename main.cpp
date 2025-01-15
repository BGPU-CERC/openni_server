#include "./header.h"

int main(int argc, char *argv[])
{
  try
  {
    astra_check(astra::initialize());

    astra::StreamSet streamSet;
    astra::StreamReader reader = streamSet.create_reader();
    
    astra::DepthStream depthStream = reader.stream<astra::DepthStream>();
    depthStream.enable_mirroring(true);
    depthStream.enable_registration(true);
    depthStream.start();

    astra::ColorStream colorStream = reader.stream<astra::ColorStream>();
    colorStream.enable_mirroring(true);
    colorStream.start();

    stream_server_start();
    while (true)
      stream(reader, depthStream, colorStream);
  }
  catch (const std::exception &e)
  {
    printf("Exception: %s\n", e.what());
    astra_check(astra::terminate());
    return -1;
  }
}
