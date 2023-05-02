#ifndef IOSTRUCTS_HPP
#define IOSTRUCTS_HPP

#include <string>
#include <iosfwd>

namespace teslenko
{
  struct DelimiterIO
  {
    char exp;
  };

  struct UllBinIO
  {
    unsigned long long& ref;
  };

  struct RatLspIO
  {
    std::pair< long long, unsigned long long >& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct LabelIO
  {
    std::string exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, UllBinIO&& dest);
  std::istream& operator>>(std::istream& in, RatLspIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, LabelIO &&dest);
}

#endif
