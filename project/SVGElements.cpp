#include "SVGElements.hpp"

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // Ellipse (initial code provided)
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius)
        : fill(fill), center(center), radius(radius)
    {
    }
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }
    Point Ellipse::get_center() const
    {
        return center;
    }
    Point Ellipse::get_radius() const
    {
        return radius;
    }

    Circle::Circle(const Color &fill, 
                   const Point &center,
                   const Point &radius)
        : Ellipse(fill, center, radius)
    {
    }
    double Circle::get_radius() const
    {
        return radius.x;
    }
    void Circle::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }
    // @todo provide the implementation of SVGElement derived classes
    // HERE -->

}
