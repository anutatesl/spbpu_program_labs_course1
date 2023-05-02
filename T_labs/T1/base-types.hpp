#ifndef BASE_TYPES_HPP
#define BASE_TYPES_HPP

#include <iosfwd>

namespace teslenko
{
  struct point_t
  {
    double x;
    double y;
  };

  struct rectangle_t
  {
    point_t pos;
    double width;
    double height;
  };

  std::ostream& operator<<(std::ostream& out, const point_t& point);
  std::ostream& operator<<(std::ostream& out, const rectangle_t& rectangle);
  point_t scalePoint(const point_t& point, const point_t& center, const double k);
  point_t& operator+=(point_t& a, const point_t& b);
  point_t& operator-=(point_t& a, const point_t& b);
  point_t& operator*=(point_t& a, double b);
  bool operator==(const point_t& a, const point_t& b);
  point_t operator*(const point_t& a, double b);
  point_t operator-(const point_t& a, const point_t& b);
  point_t operator+(const point_t& a, const point_t& b);
}

#endif
