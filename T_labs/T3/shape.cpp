#include "shape.hpp"
#include <iostream>
#include <iostructs.hpp>
#include <IofmtGuard.hpp>

std::istream& teslenko::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Point newPoint;
  in >> DelimiterIO{'('};
  in >> newPoint.x >> DelimiterIO{';'} >> newPoint.y;
  in >> DelimiterIO{')'};
  if (in)
  {
    point = newPoint;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream& teslenko::operator<<(std::ostream& out, const Point& point)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << '(' << point.x << ';' << point.y << ')';
}

std::istream& teslenko::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t amountPoints;
  in >> amountPoints;
  if (amountPoints < 3)
  {
    in.setstate(std::ios::failbit);
  }
  IofmtGuard ioftmguard(in);
  in >> std::noskipws;
  Polygon newPolygon;
  for (size_t i = 0; i < amountPoints; i++)
  {
    in >> DelimiterIO{' '};
    Point newPoint;
    in >> newPoint;
    newPolygon.points.push_back(newPoint);
  }
  in >> DelimiterIO{'\n'};
  if (in)
  {
    polygon = newPolygon;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream& teslenko::operator<<(std::ostream& out, const Polygon& polygon)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << polygon.points.size();
  for (size_t i = 0; i < polygon.points.size(); i++)
  {
    out << ' ' << polygon.points[0]; // i
  }
  return out;
}

teslenko::Point teslenko::operator+(const Point& a, const Point& b)
{
  return { a.x + b.x, a.y + b.y };
}

bool teslenko::operator==(const Polygon& a, const Polygon& b)
{
    return a.points == b.points;
}
