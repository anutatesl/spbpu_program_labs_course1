#include "polygon.hpp"
#include <memory>
#include <stdexcept>
#include <cmath>
#include "base-types.hpp"

teslenko::Polygon::Polygon(std::unique_ptr< point_t[] >&& points, size_t count):
  points_(std::move(points)),
  count_(count)
{
  if (count < 3)
  {
    throw std::logic_error("Not enought points");
  }
  for (size_t i = 0; i < count - 1; i++)
  {
    for (size_t j = i + 1; j < count; j++)
    {
      if (points_[i].x == points_[j].x && points_[i].y == points_[j].y)
      {
        throw std::logic_error("Polygon have same points");
      }
    }
  }
}

double teslenko::Polygon::getArea() const
{
  double area = 0.0;
  for (size_t i = 0; i < count_; i++)
  {
    if (i == count_ - 1)
    {
      area += (points_[i].x + points_[0].x) * (points_[0].y - points_[i].y);
    }
    else
    {
      area += (points_[i].x + points_[i + 1].x) * (points_[i + 1].y - points_[i].y);
    }
  }
  area = std::abs(area / 2.0);
  return area;
}

teslenko::rectangle_t teslenko::Polygon::getFrameRect() const
{
  double minX = points_[0].x;
  double maxX = points_[0].x;
  double minY = points_[0].y;
  double maxY = points_[0].y;
  for (size_t i = 1; i < count_; i++)
  {
    point_t temp = points_[i];
    minX = std::min(temp.x, minX);
    maxX = std::max(temp.x, maxX);
    minY = std::min(temp.y, minY);
    maxY = std::max(temp.y, maxY);
  }
  rectangle_t rectangle;
  rectangle.width = maxX - minX;
  rectangle.height = maxY - minY;
  rectangle.pos.x = (maxX + minX) / 2;
  rectangle.pos.y = (maxY + minY) / 2;
  return rectangle;
}

void teslenko::Polygon::move(const point_t& posNew)
{
  point_t center = getCenter();
  double dx = posNew.x - center.x;
  double dy = posNew.y - center.y;
  move(dx, dy);
}

void teslenko::Polygon::move(double dx, double dy)
{
  for (size_t i = 0; i < count_; ++i)
  {
    points_[i].x += dx;
    points_[i].y += dy;
  }
}

void teslenko::Polygon::doScale(double k)
{
  point_t center = getCenter();
  for (size_t i = 0; i < count_; i++)
  {
    points_[i] = scalePoint(points_[i], center, k);
  }
}

teslenko::point_t teslenko::Polygon::getCenter() const
{
  point_t center{0, 0};
  for (size_t i = 0; i < count_; ++i)
  {
    center.x += points_[i].x;
    center.y += points_[i].y;
  }
  center.x = center.x / count_;
  center.y = center.y / count_;
  return center;
}
