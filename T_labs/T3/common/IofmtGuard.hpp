#ifndef IOFMTGUARD_HPP
#define IOFMTGUARD_HPP
#include <ios>

namespace teslenko
{
  class IofmtGuard
  {
    public:
      IofmtGuard(std::basic_ios< char >& stream);
      ~IofmtGuard();

    private:
      std::basic_ios< char >& stream_;
      char fill_;
      std::streamsize precision_;
      std::basic_ios< char >::fmtflags fmt_;
  };
}

#endif
