#include <iostream>
#include "Tcp.h"

int main (int argc, char *argv[]) {
  TCP::TcpServer* server = new TCP::TcpServer(5);
  TCP::AbstractTcpMessageProcessor* logger = new TCP::TcpLogger();
  server->addProcessor(logger);
  server->start();
}
