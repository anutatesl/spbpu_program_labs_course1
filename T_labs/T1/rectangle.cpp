#include "rectangle.hpp"
#include <stdexcept>
#include <cmath>
#include "base-types.hpp"

teslenko::Rectangle::Rectangle(const rectangle_t& rectangle):
  rectangle_(rectangle)
{
  if (rectangle.width <= 0 || rectangle.height <= 0)
  {
    throw std::logic_error("Rectangle incorrect");
  }
}

teslenko::Rectangle::Rectangle(const point_t& lowerLeft, const point_t& upperRight)
{
  if (lowerLeft.x >= upperRight.x || lowerLeft.y >= upperRight.y)
  {
    throw std::logic_error("Rectangle incorrect");
  }
  rectangle_.pos.x = (lowerLeft.x + upperRight.x) / 2;
  rectangle_.pos.y = (lowerLeft.y + upperRight.y) / 2;
  rectangle_.width = upperRight.x - lowerLeft.x;
  rectangle_.height = upperRight.y - lowerLeft.y;
}

double teslenko::Rectangle::getArea() const
{
  return rectangle_.width * rectangle_.height;
}

teslenko::rectangle_t teslenko::Rectangle::getFrameRect() const
{
  return rectangle_;
}

void teslenko::Rectangle::move(const point_t& pos)
{
  rectangle_.pos = pos;
}

void teslenko::Rectangle::move(double dx, double dy)
{
  rectangle_.pos.x += dx;
  rectangle_.pos.y += dy;
}

void teslenko::Rectangle::doScale(double k)
{
  rectangle_.width *= k;
  rectangle_.height *= k;
}
