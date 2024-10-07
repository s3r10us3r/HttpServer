#include <iostream>
#include <cstring>
#include <unistd.h>
#include "Tcp.h"


TCP::TcpServer::TcpServer(int backlog_size) {
  //AF_INET - IPv4 protocol family, SOCK_STREAM defines TCP type socket
  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8080); //httons converts from uint from machine byte order to network byte order
  serverAddress.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY makes the server listen on any address
  bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
  listen(serverSocket, backlog_size);
}

void TCP::TcpServer::addProcessor(ITcpMessageProcessor* processor) {
  processors.push_back(processor);
}

void TCP::TcpServer::start() {
  while (true) {
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    std::string clientString = handleClient(clientSocket);
    close(clientSocket);
    for(auto processor : *processors) {
      processor->process(clientString);
    }
  }
}

std::string TCP::TcpServer::handleClient(int clientSocket) {
  char* buffer = new char[1025];
  std::string messageBuffer = "";
  int bytesRead;
  while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0 ) {
    buffer[bytesRead] = '\0';
    messageBuffer.append(buffer);

    if (messageBuffer.find("\n\r\n\r") != std::string::npos)
      break;
  }

  if (bytesRead < 0){
    perror("recv");
  }

  return messageBuffer;
}


void TCP::TcpLogger::process(std::string message) {
  std::cout << message;
}

ITcpMessageProcessor* TCP::TcpLogger::clone() {
  return new TCP::TcpLogger();
}

