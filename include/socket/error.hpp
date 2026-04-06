#pragma once
#include <string_view>

namespace Socket
{

enum class Error
{
  UNSPECIFIED,

  INVALID_ADDRESS_FAMILY,            // EAFNOSUPPORT
  TYPE_OR_PROTOCOL_DENIED,           // EACCES
  PROCESS_FILE_DESCRIPTOR_LIST_FULL, // EMFILE
  SYSTEM_FILE_DESCRIPTOR_LIST_FULL,  // ENFILE
  INSUFFICIENT_BUFFER_SPACE,         // ENOBUFS
  INSUFFICIENT_MEMORY,               // ENOMEM
  PROTOCOL_NOT_SUPPORTED,            // EPROTONOSUPPORT
  SOCKET_TYPE_NOT_SUPPORTED,         // EPROTOTYPE

  ADDRESS_IN_USE,                    // EADDRINUSE
  ADDRESS_NOT_AVAILABLE,             // EADDRNOTAVAIL

  CONNECTION_ALREADY_IN_PROGRESS,    // EALREADY
  CONNECTION_IN_PROGRESS,            // EINPROGRESS
  SOCKET_ALREADY_CONNECTED,          // EISCONN
  SOCKET_NOT_CONNECTED,              // ENOTCONN

  CONNECTION_REFUSED,                // ECONNREFUSED
  CONNECTION_RESET,                  // ECONNRESET
  CONNECTION_TIMED_OUT,              // ETIMEDOUT
  CONNECTION_CLOSED,                 // Return value of 0

  HOST_UNREACHABLE,                  // EHOSTUNREACH
  NETWORK_UNREACHABLE,               // ENETUNREACH

  BROKEN_PIPE,                       // EPIPE

  INVALID_FILE_DESCRIPTOR,           // EBADF
  INVALID_ARGUMENT,                  // EINVAL
  NOT_A_SOCKET,                      // ENOTSOCK
  OPERATION_NOT_SUPPORTED,           // EOPNOTSUPP

  WOULD_BLOCK                        // EWOULDBLOCK / EAGAIN
};

Error parse_errno();

} // namespace Socket

std::string_view to_string(Socket::Error err);
