#include <peterfh.hpp>
#pragma once
#include <socket/error.hpp>
#include <socket/tcp.hpp>
#include <poll.h>

namespace peterfh
{

namespace socket
{

enum class PollEvent : short
{
  Readable = POLLIN,
  Writable = POLLOUT,
  Error    = POLLERR
};

struct PollResult
{
  int m_fd;
  PollEvent m_revent;
};

class Poll
{
public:
  void add(Tcp& socket, PollEvent event);
  void remove(Tcp& socket);
  Result<PollEvent, Error> wait(Tcp& socket, PollEvent event, int timeout_ms = -1);
  Result<std::vector<PollResult>, Error> wait(int timeout_ms = -1);

private:
  std::vector<int> m_sockets;
  std::vector<struct pollfd> m_fds;
};

}

}
