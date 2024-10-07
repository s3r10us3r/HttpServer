#pragma once
#include <vector>
#include <string>

#include "sys/socket.h"
#include "netinet/in.h"

namespace TCP {

  class AbstractTcpMessageProcessor {
    public:
      virtual void process(std::string buffer) = 0;
      ITcpMessageProcessor* clone() = 0;
  };

  class TcpServer {
  private:
    int serverSocket;
    sockaddr_in serverAddress;
    std::vector<ITcpMessageProcessor*> processors;
    std::string handleClient(int clientSocket);
  public:
    TcpServer(int backlog_size); 
    void addProcessor(ITcpMessageProcessor* processor);
    void start();
  };

  class TcpLogger : public AbstractTcpMessageProcessor {
    public:
      void process(std::string message);
      ITcpMessageProcessor* clone();
  };

  class TcpConnection {
    private:
      int clientSocket;
      bool keepAlive;
      std::vector<ITcpMessageProcessor*> processors;
    public:
      TcpConnection(int clientSocket, bool keepAlive, std::vector<ITcpMessageProcessor*>& processors,
          void (*onCloseCallback)(TcpConnection*) onCloseCallback);
      ~TcpConnection();
  }
}

