#include <http/client.hpp>
#include <socket/tcp.hpp>

namespace peterfh::http
{

Result<std::vector<std::byte>, Error> send(Request req)
{
  socket::Address addr = req.url().addr();
  socket::Tcp sock;
  const auto connect_result = sock.connect(addr);
  if (!connect_result.successful())
  {
    return Error::CONNECTION_REFUSED;
  }
  const auto send_result = sock.send(req.data());
  if (!send_result.successful())
  {
    return Error::SEND_CANCELLED;
  }
  const auto res = sock.receive_available();
  if (!res.successful())
  {
    return Error::RECEIVE_FAILED;
  }
  if (!res->second)
  {
    return Error::RECEIVE_PARTIAL;
  }

  return res->first;
}

}
