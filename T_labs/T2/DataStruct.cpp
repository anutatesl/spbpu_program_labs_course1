#include "DataStruct.hpp"
#include <iomanip>
#include "iostructs.hpp"
#include "IofmtGuard.hpp"

bool teslenko::operator<(const DataStruct& data1, const DataStruct& data2)
{
  if (data1.key1 < data2.key1)
  {
    return true;
  }
  else if (data1.key1 == data2.key1)
  {
    if (data1.key2 < data2.key2)
    {
      return true;
    }
    else if (data1.key2 == data2.key2)
    {
      if (data1.key3.size() < data2.key3.size())
      {
        return true;
      }
    }
  }
  return false;
}

std::istream& teslenko::operator>>(std::istream& in, teslenko::DataStruct& data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  {
    using sep = DelimiterIO;
    using ullBin = UllBinIO;
    using ratLsp = RatLspIO;
    using str = StringIO;
    using label = LabelIO;
    in >> sep{'('} >> sep{':'};
    for (int i = 0; i < 3; i++)
    {
      in >> label{"key"};
      char ch = ' ';
      in >> ch;
      if (ch == '1')
      {
        in >> ullBin{input.key1};
      }
      else if (ch == '2')
      {
        in >> ratLsp{input.key2};
      }
      else if (ch == '3')
      {
        in >> str{input.key3};
      }
      else
      {
        in.setstate(std::ios::failbit);
      }
      in >> sep{':'};
    }
    in >> sep{')'};
  }
  if (in)
  {
    data = input;
  }
  return in;
}

namespace teslenko
{
  std::string convertToBinStr(unsigned long long number);
}

std::ostream& teslenko::operator <<(std::ostream& out, const teslenko::DataStruct& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  IofmtGuard fmtguard(out);
  out << "(:key1 0b0" << convertToBinStr(data.key1);
  out << ":key2 (:N " << data.key2.first << ":D " << data.key2.second << ":)";
  out << ":key3 \"" << data.key3 << "\":)";
  return out;
}

namespace teslenko
{
  std::string convertToBinStr(unsigned long long number)
  {
    std::string result = "";
    while (number > 0)
    {
      unsigned long long temp = number % 2;
      if (temp == 1)
      {
        result += "1";
      }
      else
      {
        result += "0";
      }
      number /= 2;
    }
    return result;
  }
}
