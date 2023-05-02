#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "shape.hpp"
#include "base-types.hpp"

namespace teslenko
{
  class Rectangle: public Shape
  {
    public:
      Rectangle(const rectangle_t& rectangle);
      Rectangle(const point_t& lowerLeft, const point_t& upperRight);
      ~Rectangle() = default;

      double getArea() const;
      rectangle_t getFrameRect() const;
      void move(const point_t& pos);
      void move(double dx, double dy);
      void doScale(double k);

    private:
      rectangle_t rectangle_;
  };
}

#endif
