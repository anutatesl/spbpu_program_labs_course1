#ifndef IOSTRUCTS_HPP
#define IOSTRUCTS_HPP

#include <iosfwd>

namespace teslenko
{
  struct DelimiterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}

#endif
