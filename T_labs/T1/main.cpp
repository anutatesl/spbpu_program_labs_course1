#include <iostream>
#include <memory>
#include <iomanip>
#include <string>
#include <functional>
#include "base-types.hpp"
#include "shape.hpp"
#include "polygon.hpp"
#include "diamond.hpp"
#include "rectangle.hpp"

namespace teslenko
{
  std::string getTheLineElem(std::string& line)
  {
    size_t nextSpace = line.find_first_of(' ', 0);
    std::string nameGeometricShape = line.substr(0, nextSpace);
    nextSpace = (nextSpace == line.npos) ? nextSpace : nextSpace + 1;
    line.erase(0, nextSpace);
    return nameGeometricShape;
  }

  point_t takePointFromString(std::string& line)
  {
    std::string x = getTheLineElem(line);
    std::string y = getTheLineElem(line);
    if (x.empty() || y.empty())
    {
      throw std::logic_error("String point incorrect");
    }
    return point_t{ std::stod(x), std::stod(y) };
  }

  static const double GOLDEN_RATIO = 1.6180339887;

  template< typename T >
  std::unique_ptr< T[] > add(std::unique_ptr< T[] >&& old, size_t& oldCount, size_t& oldCapacity, T&& toAdd)
  {
    if (oldCount < oldCapacity)
    {
      std::unique_ptr< T[] > newArray = std::move(old);
      newArray[oldCount++] = std::move(toAdd);
      return newArray;
    }
    size_t newCapacity = oldCapacity * GOLDEN_RATIO + 1;
    std::unique_ptr< T[] > newArray = std::make_unique< T[] >(newCapacity);
    for (size_t i = 0; i < oldCount; ++i)
    {
      newArray[i] = std::move(old[i]);
    }
    newArray[oldCount++] = std::move(toAdd);
    oldCapacity = newCapacity;
    return newArray;
  }

  std::ostream& printShapes(std::ostream& out, std::unique_ptr< std::unique_ptr< Shape >[] >&& shapes, size_t size)
  {
    if (size == 0)
    {
      throw std::logic_error("Not shapes");
    }
    double areaShapes = 0;
    for (size_t i = 0; i < size; i++)
    {
      areaShapes += shapes[i]->getArea();
    }
    out << areaShapes;
    for (size_t i = 0; i < size; i++)
    {
      out << ' ' << shapes[i]->getFrameRect();
    }
    out << '\n';
    return out;
  }

  void scale(const point_t& scalePos, double k, std::unique_ptr<Shape>& shapes)
  {
    if (k <= 0.0)
    {
      throw std::logic_error("Invalid scaling factor");
    }
    point_t center1 = shapes->getFrameRect().pos;
    shapes->move(scalePos);
    point_t center2 = shapes->getFrameRect().pos;
    double x = center2.x - center1.x;
    double y = center2.y - center1.y;
    shapes->scale(k);
    shapes->move(-x * k, -y * k);
  }
}

int main()
{
  size_t size = 0;
  size_t capacity = 0;
  std::cout << std::fixed << std::setprecision(1);
  using ushape_t = std::unique_ptr< teslenko::Shape >;
  std::unique_ptr<  ushape_t[] > shapes = std::make_unique< ushape_t[] >(capacity);
  std::string line = "";
  std::string nameGeometricShape = "";
  bool existenceScale = false;
  while (!std::cin.eof())
  {
    try
    {
      std::getline(std::cin, line);
      if (line.size())
      {
        nameGeometricShape = teslenko::getTheLineElem(line);
        if (nameGeometricShape == "RECTANGLE")
        {
          teslenko::point_t lowerLeft = teslenko::takePointFromString(line);
          teslenko::point_t upperRight = teslenko::takePointFromString(line);
          ushape_t shape = std::make_unique< teslenko::Rectangle >(lowerLeft, upperRight);
          shapes = teslenko::add< ushape_t >(std::move(shapes), size, capacity, std::move(shape));
        }
        else if (nameGeometricShape == "DIAMOND")
        {
          teslenko::point_t point1 = teslenko::takePointFromString(line);
          teslenko::point_t point2 = teslenko::takePointFromString(line);
          teslenko::point_t point3 = teslenko::takePointFromString(line);
          ushape_t shape = std::make_unique< teslenko::Diamond >(point1, point2, point3);
          shapes = teslenko::add< ushape_t >(std::move(shapes), size, capacity, std::move(shape));
        }
        else if (nameGeometricShape == "POLYGON")
        {
          size_t sizePoints = 0;
          size_t capacityPoints = 0;
          std::unique_ptr< teslenko::point_t[] > points = nullptr;
          while (!line.empty())
          {
            teslenko::point_t newPoint = teslenko::takePointFromString(line);
            points = teslenko::add< teslenko::point_t >(std::move(points), sizePoints, capacityPoints, std::move(newPoint));
          }
          ushape_t shape = std::make_unique< teslenko::Polygon >(std::move(points), sizePoints);
          shapes = teslenko::add< ushape_t >(std::move(shapes), size, capacity, std::move(shape));
        }
        else if (nameGeometricShape == "SCALE")
        {
          teslenko::point_t scalePos = teslenko::takePointFromString(line);
          double k = std::stod(line);
          teslenko::printShapes(std::cout, std::move(shapes), size);
          for (size_t i = 0; i < size; ++i)
          {
            teslenko::scale(scalePos, k, shapes[i]);
          }
          teslenko::printShapes(std::cout, std::move(shapes), size);
          existenceScale = true;
          break;
        }
        else
        {
          continue;
        }
      }
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << '\n';
    }
  }
  if (size == 0 || !existenceScale)
  {
    std::cerr << "Arguments is not correct";
    return 1;
  }
  return 0;
}
