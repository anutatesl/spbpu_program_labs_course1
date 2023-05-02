#ifndef DIAMOND_HPP
#define DIAMOND_HPP

#include <memory>
#include "shape.hpp"
#include "base-types.hpp"

namespace teslenko
{
  class Diamond: public Shape
  {
    public:
      Diamond(const point_t& point1, const point_t& point2, const point_t& point3);
      ~Diamond() = default;

      double getArea() const;
      rectangle_t getFrameRect() const;
      void move(const point_t& pos);
      void move(double dx, double dy);
      void doScale(double k);

    private:
      point_t point1_;
      point_t point2_;
      point_t point3_;

      double foundMaxLength(const double& a, const double& b, const double& c) const;
      double getLength(const point_t& point1, const point_t& point2) const;

      point_t getCenterPoint(const point_t& a, const point_t& b, const point_t& c) const;
      point_t getPointDiagonalX(const point_t& a, const point_t& b, const point_t& c) const;
      point_t getPointDiagonalY(const point_t& a, const point_t& b, const point_t& c) const;
  };
}

#endif
