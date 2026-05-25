#include <socket/poll.hpp>

namespace peterfh::socket
{

void Poll::add(Tcp& socket, PollEvent event)
{
  pollfd pf;
  {
    pf.fd = socket.handle();
    pf.events = static_cast<short>(event);
    pf.revents = 0;
  }
  m_sockets.push_back(socket.handle());
  m_fds.push_back(pf);
}

void Poll::remove(Tcp& socket)
{
  auto socket_it = m_sockets.begin();
  auto fd_it = m_fds.begin();
  while (socket_it != m_sockets.end())
  {
    if (*socket_it == socket.handle())
    {
      m_sockets.erase(socket_it);
      m_fds.erase(fd_it);
    }
    else
    {
      ++socket_it;
      ++fd_it;
    }
  }
}

Result<PollEvent, Error> Poll::wait(Tcp& socket, PollEvent event, int timeout_ms)
{
  pollfd pf[1];
  {
    pf[0].fd = socket.handle();
    pf[0].events = static_cast<short>(event);
    pf[0].revents = 0;
  }

  int ret = ::poll(pf, 1, timeout_ms);
  if (ret < 0)
  {
    return parse_errno();
  }
  if (ret == 0)
  {
    return Error::TIMEOUT_REACHED;
  }

  return static_cast<PollEvent>(pf[0].revents);

}

} // namespace peterfh::socket

