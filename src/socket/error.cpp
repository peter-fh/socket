#include <socket/error.hpp>
#include <cerrno>

namespace peterfh::socket
{

Error parse_errno()
{
  switch (errno)
  {
    case EAFNOSUPPORT:
      return Error::INVALID_ADDRESS_FAMILY;
    case EACCES:
      return Error::TYPE_OR_PROTOCOL_DENIED;
    case EMFILE:
      return Error::PROCESS_FILE_DESCRIPTOR_LIST_FULL;
    case ENFILE:
      return Error::SYSTEM_FILE_DESCRIPTOR_LIST_FULL;
    case ENOBUFS:
      return Error::INSUFFICIENT_BUFFER_SPACE;
    case ENOMEM:
      return Error::INSUFFICIENT_MEMORY;
    case EPROTONOSUPPORT:
      return Error::PROTOCOL_NOT_SUPPORTED;
    case EPROTOTYPE:
      return Error::SOCKET_TYPE_NOT_SUPPORTED;

    case EADDRINUSE:
      return Error::ADDRESS_IN_USE;
    case EADDRNOTAVAIL:
      return Error::ADDRESS_NOT_AVAILABLE;
    case EALREADY:
      return Error::CONNECTION_ALREADY_IN_PROGRESS;
    case EINPROGRESS:
      return Error::CONNECTION_IN_PROGRESS;
    case EISCONN:
      return Error::SOCKET_ALREADY_CONNECTED;
    case ENOTCONN:
      return Error::SOCKET_NOT_CONNECTED;
    case ECONNREFUSED:
      return Error::CONNECTION_REFUSED;
    case ECONNRESET:
      return Error::CONNECTION_RESET;
    case ETIMEDOUT:
      return Error::CONNECTION_TIMED_OUT;
    case EHOSTUNREACH:
      return Error::HOST_UNREACHABLE;
    case ENETUNREACH:
      return Error::NETWORK_UNREACHABLE;
    case EPIPE:
      return Error::BROKEN_PIPE;
    case EBADF:
      return Error::INVALID_FILE_DESCRIPTOR;
    case EINVAL:
      return Error::INVALID_ARGUMENT;
    case ENOTSOCK:
      return Error::NOT_A_SOCKET;
    case EOPNOTSUPP:
      return Error::OPERATION_NOT_SUPPORTED;
    case EWOULDBLOCK:
      return Error::WOULD_BLOCK;

    default:
      return Error::UNSPECIFIED;
  }
}

template <typename T>
Result<T, Error> construct_errno_result()
{
  const Error err = parse_errno();
  return Result<T, Error>(err);
}

} // namespace Socket

namespace peterfh
{
std::string_view to_string(socket::Error err)
{
  using namespace std::string_view_literals;

  switch (err)
  {
    case socket::Error::UNSPECIFIED:
      return "UNSPECIFIED"sv;

    case socket::Error::INVALID_ADDRESS_FAMILY:
      return "INVALID_ADDRESS_FAMILY [EAFNOSUPPORT]"sv;

    case socket::Error::TYPE_OR_PROTOCOL_DENIED:
      return "TYPE_OR_PROTOCOL_DENIED [EACCES]"sv;

    case socket::Error::PROCESS_FILE_DESCRIPTOR_LIST_FULL:
      return "PROCESS_FILE_DESCRIPTOR_LIST_FULL [EMFILE]"sv;

    case socket::Error::SYSTEM_FILE_DESCRIPTOR_LIST_FULL:
      return "SYSTEM_FILE_DESCRIPTOR_LIST_FULL [ENFILE]"sv;

    case socket::Error::INSUFFICIENT_BUFFER_SPACE:
      return "INSUFFICIENT_BUFFER_SPACE [ENOBUFS]"sv;

    case socket::Error::INSUFFICIENT_MEMORY:
      return "INSUFFICIENT_MEMORY [ENOMEM]"sv;

    case socket::Error::PROTOCOL_NOT_SUPPORTED:
      return "PROTOCOL_NOT_SUPPORTED [EPROTONOSUPPORT]"sv;

    case socket::Error::SOCKET_TYPE_NOT_SUPPORTED:
      return "SOCKET_TYPE_NOT_SUPPORTED [EPROTOTYPE]"sv;

    case socket::Error::ADDRESS_IN_USE:
      return "ADDRESS_IN_USE [EADDRINUSE]"sv;

    case socket::Error::ADDRESS_NOT_AVAILABLE:
      return "ADDRESS_NOT_AVAILABLE [EADDRNOTAVAIL]"sv;

    case socket::Error::CONNECTION_ALREADY_IN_PROGRESS:
      return "CONNECTION_ALREADY_IN_PROGRESS [EALREADY]"sv;

    case socket::Error::CONNECTION_IN_PROGRESS:
      return "CONNECTION_IN_PROGRESS [EINPROGRESS]"sv;

    case socket::Error::SOCKET_ALREADY_CONNECTED:
      return "SOCKET_ALREADY_CONNECTED [EISCONN]"sv;

    case socket::Error::SOCKET_NOT_CONNECTED:
      return "SOCKET_NOT_CONNECTED [ENOTCONN]"sv;

    case socket::Error::CONNECTION_REFUSED:
      return "CONNECTION_REFUSED [ECONNREFUSED]"sv;

    case socket::Error::CONNECTION_RESET:
      return "CONNECTION_RESET [ECONNRESET]"sv;

    case socket::Error::CONNECTION_TIMED_OUT:
      return "CONNECTION_TIMED_OUT [ETIMEDOUT]"sv;

    case socket::Error::HOST_UNREACHABLE:
      return "HOST_UNREACHABLE [EHOSTUNREACH]"sv;

    case socket::Error::NETWORK_UNREACHABLE:
      return "NETWORK_UNREACHABLE [ENETUNREACH]"sv;

    case socket::Error::BROKEN_PIPE:
      return "BROKEN_PIPE [EPIPE]"sv;

    case socket::Error::INVALID_FILE_DESCRIPTOR:
      return "INVALID_FILE_DESCRIPTOR [EBADF]"sv;

    case socket::Error::INVALID_ARGUMENT:
      return "INVALID_ARGUMENT [EINVAL]"sv;

    case socket::Error::NOT_A_SOCKET:
      return "NOT_A_SOCKET [ENOTSOCK]"sv;

    case socket::Error::OPERATION_NOT_SUPPORTED:
      return "OPERATION_NOT_SUPPORTED [EOPNOTSUPP]"sv;

    case socket::Error::WOULD_BLOCK:
      return "WOULD_BLOCK [EWOULDBLOCK/EAGAIN]"sv;

    case socket::Error::CONNECTION_CLOSED:
      return "CONNECTION_CLOSED"sv;
  }

  return "UNKNOWN"sv;
}
}
