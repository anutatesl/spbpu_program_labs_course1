#include "commands.hpp"
#include <algorithm>
#include <string>
#include <iostream>
#include <iomanip>
#include <functional>
#include <stdexcept>
#include <numeric>
#include <IofmtGuard.hpp>

namespace teslenko
{
  bool isNumber(const std::string& s)
  {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
  }

  double doPolygonArea(double area, const Point& point, Point& prevPoint)
  {
    area += (prevPoint.x + point.x) * (prevPoint.y - point.y);
    prevPoint = point;
    return area;
  }

  double getAreaPolygon(const Polygon& polygon)
  {
    auto begin = polygon.points.begin();
    auto end = polygon.points.end();
    Point prevPoint = polygon.points[polygon.points.size() - 1];
    using namespace std::placeholders;
    double area = std::accumulate(begin, end, 0.0, std::bind(doPolygonArea, _1, _2, prevPoint));
    return std::abs(area / 2.0);
  }

  bool countEvenOrOdd(const Polygon& polygon, double area, bool isEven)
  {
    if (isEven)
    {
      if (polygon.points.size() % 2 != 0)
      {
        return area + getAreaPolygon(polygon);
      }
      return area;
    }
    else
    {
      if (polygon.points.size() % 2 == 0)
      {
        return area + getAreaPolygon(polygon);
      }
      return area;
    }
  }

  double getAreaEven(const std::vector< Polygon >& polygons)
  {
    using namespace std::placeholders;
    return std::accumulate(polygons.begin(), polygons.end(), 0.0, std::bind(countEvenOrOdd, _1, _2, true));
  }

  double getAreaOdd(const std::vector< Polygon >& polygons)
  {
    using namespace std::placeholders;
    return std::accumulate(polygons.begin(), polygons.end(), 0.0, std::bind(countEvenOrOdd, _1, _2, false));
  }

  double getAreaMean(const std::vector< Polygon >& polygons)
  {
    if (polygons.begin() == polygons.end())
    {
      throw std::invalid_argument("Not enought shapes");
    }
    double result = getAreaEven(polygons) + getAreaOdd(polygons);
    return result / polygons.size();
  }

  double sumVer(double area, const Polygon& polygon, const size_t& n)
  {
    if (polygon.points.size() == n)
    {
      return area + getAreaPolygon(polygon);
    }
    return area;
  }

  double getAreaNumOfVertexes(const std::vector< Polygon >& polygons, const size_t& n)
  {
    if (n < 3)
    {
      throw std::logic_error("Wrong polygon");
    }
    using namespace std::placeholders;
    double result = std::accumulate(polygons.begin(), polygons.end(), 0.0, std::bind(sumVer, _1, _2, n));
    return result;
  }

  double compareArea(const Polygon& polygon1, const Polygon& polygon2)
  {
    return getAreaPolygon(polygon1) < getAreaPolygon(polygon2);
  }

  double getMaxArea(const std::vector< Polygon >& polygons)
  {
    if (polygons.begin() == polygons.end())
    {
      throw std::invalid_argument("Not enought shapes");
    }
    using namespace std::placeholders;
    Polygon maxPolygon = *std::max_element(polygons.begin(), polygons.end(), std::bind(compareArea, _1, _2));
    return getAreaPolygon(maxPolygon);
  }

  double compareVertexes(const Polygon& polygon1, const Polygon& polygon2)
  {
    return polygon1.points.size() < polygon2.points.size();
  }

  double getMaxVertexes(const std::vector< Polygon >& polygons)
  {
    if (polygons.begin() == polygons.end())
    {
      throw std::invalid_argument("Not enought shapes");
    }
    using namespace std::placeholders;
    Polygon polygonMax = *std::max_element(polygons.begin(), polygons.end(), std::bind(compareVertexes, _1, _2));
    return polygonMax.points.size();
  }

  double getMinArea(const std::vector< Polygon >& polygons)
  {
    if (polygons.begin() == polygons.end())
    {
      throw std::invalid_argument("Not enought shapes");
    }
    using namespace std::placeholders;
    Polygon minPolygon = *std::min_element(polygons.begin(), polygons.end(), std::bind(compareArea, _1, _2));
    return getAreaPolygon(minPolygon);
  }

  double getMinVertexes(const std::vector< Polygon >& polygons)
  {
    if (polygons.begin() == polygons.end())
    {
      throw std::invalid_argument("Not enought shapes");
    }
    using namespace std::placeholders;
    Polygon polygonMin = *std::min_element(polygons.begin(), polygons.end(), std::bind(compareVertexes, _1, _2));
    return polygonMin.points.size();
  }

  size_t getCountEven(const std::vector< Polygon >& polygons)
  {
    using namespace std::placeholders;
    return std::count_if(polygons.begin(), polygons.end(), std::bind(countEvenOrOdd, _1, _2, true));
  }

  size_t getCountOdd(const std::vector< Polygon >& polygons)
  {
    using namespace std::placeholders;
    return std::count_if(polygons.begin(), polygons.end(), std::bind(countEvenOrOdd, _1, _2, false));
  }

  bool compareSize(const Polygon& polygon, const size_t& n)
  {
    return polygon.points.size() == n;
  }

  size_t getCountNumOfVertexes(const std::vector< Polygon >& polygons, const size_t& n)
  {
    if (n < 3)
    {
      throw std::logic_error("Wrong polygon");
    }
    using namespace std::placeholders;
    size_t result = std::count_if(polygons.begin(), polygons.end(), std::bind(compareSize, _1, n));
    return result;
  }

  Point doOffset(Point point, Point offset)
  {
    return point + offset;
  }

  bool isSamePolygons(const Polygon& polygon1, const Polygon& polygon2)
  {
    std::vector< Point > first = polygon1.points;
    std::vector< Point > second = polygon2.points;
    if (first.size() != second.size())
    {
      return false;
    }
    using namespace std::placeholders;
    Point offset{ first[0].x - second[0].x, first[0].y - second[0].y };
    std::vector< Point > temp;
    std::transform(second.begin(), second.end(), std::back_inserter(temp), std::bind(doOffset, _1, offset));
    return first == temp;
  }

  size_t chooseSequence(const Polygon& polygon1, const Polygon& polygon2, size_t maxSeq, size_t seq)
  {
      if (polygon1 == polygon2)
      {
          return maxSeq++;
      }
      else if (maxSeq < seq)
      {
          maxSeq = seq;
      }
      return 0;
  }
}

teslenko::Commands::Commands(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out):
  polygons_(polygons),
  in_(in),
  out_(out)
{}

void teslenko::Commands::getArea()
{
  std::string way = "";
  in_ >> way;
  IofmtGuard ioftmguard(out_);
  out_ << std::fixed << std::setprecision(1);
  try
  {
    if (way == "EVEN")
    {
      out_ << getAreaEven(polygons_) << '\n';
    }
    else if (way == "ODD")
    {
      out_ << getAreaOdd(polygons_) << '\n';
    }
    else if (way == "MEAN")
    {
      out_ << getAreaMean(polygons_) << '\n';
    }
    else if (isNumber(way))
    {
      size_t count = std::stoul(way);
      out_ << getAreaNumOfVertexes(polygons_, count) << '\n';
    }
    else
    {
      throw std::logic_error("Error area commands");
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}

void teslenko::Commands::getMax()
{
  std::string way = "";
  in_ >> way;
  IofmtGuard ioftmguard(out_);
  out_ << std::fixed << std::setprecision(1);
  try
  {
    if (way == "AREA")
    {
      out_ << getMaxArea(polygons_) << '\n';
    }
    else if (way == "VERTEXES")
    {
      out_ << getMaxVertexes(polygons_) << '\n';
    }
    else
    {
      throw std::logic_error("Error max commands");
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}

void teslenko::Commands::getMin()
{
  std::string way = "";
  in_ >> way;
  IofmtGuard ioftmguard(out_);
  out_ << std::fixed << std::setprecision(1);
  try
  {
    if (way == "AREA")
    {
      out_ << getMinArea(polygons_) << '\n';
    }
    else if (way == "VERTEXES")
    {
      out_ << getMinVertexes(polygons_) << '\n';
    }
    else
    {
      throw std::logic_error("Error min commands");
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}

void teslenko::Commands::getCount()
{
  std::string way = "";
  in_ >> way;
  IofmtGuard ioftmguard(out_);
  out_ << std::fixed << std::setprecision(1);
  try
  {
    if (way == "EVEN")
    {
      out_ << getCountEven(polygons_) << '\n';
    }
    else if (way == "ODD")
    {
      out_ << getCountOdd(polygons_) << '\n';
    }
    else if (isNumber(way))
    {
      size_t count = std::stoul(way);
      out_ << getCountNumOfVertexes(polygons_, count) << '\n';
    }
    else
    {
      throw std::logic_error("Error count commands");
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}

void teslenko::Commands::getMaxSeq()
{
    Polygon polygon;
    in_ >> polygon;
    if (!in_)
    {
        throw std::logic_error("Error same commands");
    }

    size_t maxSeq = 0;
    using namespace std::placeholders;
    out_ << std::accumulate(polygons_.begin(), polygons_.end(), 0, std::bind(chooseSequence, _1, polygon, maxSeq, _2)) << '\n';
}

void teslenko::Commands::getSame()
{
  Polygon polygon;
  in_ >> polygon;
  if (!in_)
  {
    throw std::logic_error("Error same commands");
  }
  using namespace std::placeholders;
  out_ << std::count_if(polygons_.begin(), polygons_.end(), std::bind(isSamePolygons, polygon, _1)) << '\n';
}
