#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <memory>
#include "shape.hpp"
#include "base-types.hpp"

namespace teslenko
{
  class Polygon: public Shape
  {
    public:
      Polygon(std::unique_ptr< point_t[] >&& points, size_t count);
      ~Polygon() = default;

      double getArea() const;
      rectangle_t getFrameRect() const;
      void move(const point_t& posNew);
      void move(double dx, double dy);
      void doScale(double k);

    private:
      std::unique_ptr< point_t[] > points_;
      size_t count_;

      point_t getCenter() const;
  };
}

#endif
