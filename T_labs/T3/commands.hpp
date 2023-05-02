#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include "shape.hpp"

namespace teslenko
{
  struct  Commands
  {
    public:
      Commands(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);

      void getArea();
      void getMax();
      void getMin();
      void getCount();
      void getMaxSeq();
      void getSame();

    private:
      std::vector< Polygon > polygons_;
      std::istream& in_;
      std::ostream& out_;
  };
}

#endif
