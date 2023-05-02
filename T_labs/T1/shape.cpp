#include "shape.hpp"
#include <stdexcept>

void teslenko::Shape::scale(double k)
{
  if (k <= 0.0)
  {
    throw std::logic_error("Invalid scaling factor");
  }
  doScale(k);
}
