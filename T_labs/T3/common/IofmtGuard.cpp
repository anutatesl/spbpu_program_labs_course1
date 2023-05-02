#include "IofmtGuard.hpp"

teslenko::IofmtGuard::IofmtGuard(std::basic_ios< char >& stream):
  stream_(stream),
  fill_(stream.fill()),
  precision_(stream.precision()),
  fmt_(stream.flags())
{}

teslenko::IofmtGuard::~IofmtGuard()
{
  stream_.fill(fill_);
  stream_.precision(precision_);
  stream_.flags(fmt_);
}
