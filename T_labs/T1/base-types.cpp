#include "base-types.hpp"
#include <iostream>
#include <cmath>

std::ostream& teslenko::operator<<(std::ostream& out, const point_t& point)
{
  out << point.x << ' ' << point.y;
  return out;
}

std::ostream& teslenko::operator<<(std::ostream& out, const rectangle_t& rectangle)
{
  point_t lowerLeft = { rectangle.pos.x - rectangle.width / 2, rectangle.pos.y - rectangle.height / 2 };
  point_t upperRight = { rectangle.pos.x + rectangle.width / 2, rectangle.pos.y + rectangle.height / 2 };
  return out << lowerLeft << ' ' << upperRight;
}

teslenko::point_t teslenko::scalePoint(const point_t& point, const point_t& center, const double k)
{
  return point_t{ k * (point.x - center.x) + center.x, k * (point.y - center.y) + center.y };
}

teslenko::point_t& teslenko::operator+=(point_t& a, const point_t& b)
{
  return a = { a.x + b.x, a.y + b.y };
}

teslenko::point_t& teslenko::operator-=(point_t& a, const point_t& b)
{
  return a = { a.x - b.x, a.y - b.y };
}

teslenko::point_t& teslenko::operator*=(point_t& a, double b)
{
  return a = { a.x * b, a.y * b };
}

bool teslenko::operator==(const point_t& a, const point_t& b)
{
  return a.x == b.x && a.y == b.y;
}

teslenko::point_t teslenko::operator*(const point_t& a, double b)
{
  point_t ret = a;
  return ret *= b;
}

teslenko::point_t teslenko::operator-(const point_t& a, const point_t& b)
{
  point_t ret = a;
  return ret -= b;
}

teslenko::point_t teslenko::operator+(const point_t& a, const point_t& b)
{
  point_t ret = a;
  return ret += b;
}
