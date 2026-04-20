#include <peterfh.hpp>
#include <http/types.hpp>
#include <http/request.hpp>

namespace peterfh
{

namespace http
{

Result<std::vector<std::byte>, Error> send(Request req);

} // namespace http
} // namespace peterfh
