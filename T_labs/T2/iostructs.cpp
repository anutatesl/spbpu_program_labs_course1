#include "iostructs.hpp"
#include <string>
#include <iostream>
#include "IofmtGuard.hpp"

std::istream& teslenko::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& teslenko::operator>>(std::istream& in, UllBinIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{'0'};
  char ch = in.peek();
  std::string countBinary = "";
  if (ch == 'b' || ch == 'B')
  {
    in >> DelimiterIO{ch};
    while (in.peek() == '0' || in.peek() == '1')
    {
      countBinary += in.get();
    }
  }
  else
  {
    return in;
  }
  try
  {
    unsigned long long count = std::stoull(countBinary, nullptr, 2);
    dest.ref = count;
  }
  catch (const std::exception& ex)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!in)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& teslenko::operator>>(std::istream& in, RatLspIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{'('} >> DelimiterIO{':'} >> DelimiterIO{'N'} >> dest.ref.first;
  in >> DelimiterIO{':'} >> DelimiterIO{'D'} >> dest.ref.second >> DelimiterIO{':'} >> DelimiterIO{')'};
  return in;
}

std::istream& teslenko::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{'"'}, dest.ref, '\"');
}

std::istream& teslenko::operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  for (size_t i = 0; i < dest.exp.length(); i++)
  {
    char ch = in.peek();
    if (!std::isalpha(ch) || std::tolower(ch) != std::tolower(dest.exp[i]))
    {
      in.setstate(std::ios::failbit);
    }
    else
    {
      if (std::isupper(ch))
      {
        in >> DelimiterIO{static_cast< char >(std::toupper(dest.exp[i]))};
      }
      else
      {
        in >> DelimiterIO{static_cast< char >(std::tolower(dest.exp[i]))};
      }
    }
  }
  return in;
}
