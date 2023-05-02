#include "diamond.hpp"
#include <memory>
#include <stdexcept>
#include <cmath>
#include "base-types.hpp"

teslenko::Diamond::Diamond(const point_t& point1, const point_t& point2, const point_t& point3):
  point1_(getCenterPoint(point1, point2, point3)),
  point2_(getPointDiagonalX(point1, point2, point3)),
  point3_(getPointDiagonalY(point1, point2, point3))
{
  if (point1 == point2 || point1 == point3 || point2 == point3)
  {
    throw std::logic_error("Diamond have same points");
  }
}

double teslenko::Diamond::getArea() const
{
  double length12 = getLength(point1_, point2_);
  double length31 = getLength(point3_, point1_);
  return 2 * length12 * length31;
}

teslenko::rectangle_t teslenko::Diamond::getFrameRect() const
{
  double length12 = getLength(point1_, point2_);
  double length31 = getLength(point3_, point1_);
  rectangle_t rectangle;
  rectangle.width = 2 * length12;
  rectangle.height = 2 * length31;
  rectangle.pos = point1_;
  return rectangle;
}

void teslenko::Diamond::move(const point_t& posNew)
{
  point_t center = point1_;
  double dx = posNew.x - center.x;
  double dy = posNew.y - center.y;
  move(dx, dy);
}

void teslenko::Diamond::move(double dx, double dy)
{
  point1_.x = point1_.x + dx;
  point1_.y = point1_.y + dy;
  point2_.x = point2_.x + dx;
  point2_.y = point2_.y + dy;
  point3_.x = point3_.x + dx;
  point3_.y = point3_.y + dy;
}

void teslenko::Diamond::doScale(double k)
{
  point_t pos = point1_;
  point1_ = scalePoint(point1_, pos, k);
  point2_ = scalePoint(point2_, pos, k);
  point3_ = scalePoint(point3_, pos, k);
}

double teslenko::Diamond::foundMaxLength(const double& a, const double& b, const double& c) const
{
  double maxLength = std::max(a, b);
  maxLength = std::max(maxLength, c);
  return maxLength;
}

double teslenko::Diamond::getLength(const point_t& point1, const point_t& point2) const
{
  return std::sqrt((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y));
}

namespace tes = teslenko;

tes::point_t tes::Diamond::getCenterPoint(const point_t& a, const point_t& b, const point_t& c) const
{
  double length12 = getLength(a, b);
  double length23 = getLength(b, c);
  double length31 = getLength(c, a);
  if (!length12 || !length23 || !length31)
  {
    throw std::logic_error("It is not diamond");
  }
  double maxLenght = foundMaxLength(length12, length23, length31);
  if ((maxLenght == length12) && (c.x == a.x || c.x == b.x))
  {
    return c;
  }
  else if ((maxLenght == length23) && (a.x == b.x || a.x == c.x))
  {
    return a;
  }
  else if ((maxLenght == length31) && (b.x == a.x || b.x == c.x))
  {
    return b;
  }
  else
  {
    throw std::logic_error("It is not diamond");
  }
}

tes::point_t tes::Diamond::getPointDiagonalX(const point_t& a, const point_t& b, const point_t& c) const
{
  if (a.x == b.x)
  {
    return c;
  }
  else if (a.x == c.x)
  {
    return b;
  }
  else if (b.x == c.x)
  {
    return a;
  }
  else
  {
    throw std::logic_error("It is not correct diamond");
  }
}

tes::point_t tes::Diamond::getPointDiagonalY(const point_t& a, const point_t& b, const point_t& c) const
{
  if (a.y == b.y)
  {
    return c;
  }
  else if (a.y == c.y)
  {
    return b;
  }
  else if (b.y == c.y)
  {
    return a;
  }
  else
  {
    throw std::logic_error("It is not correct diamond");
  }
}
