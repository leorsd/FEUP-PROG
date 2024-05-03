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
                   double radius)
        : Ellipse(fill, center, Point(radius, radius))
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

    Polyline::Polyline(const Color &stroke,
                       const std::vector<Point> &points)
                     : stroke(stroke), points(points)
    {
    }

    void Polyline::draw(PNGImage &img) const
    {   
        if (points.size() >= 2)
        {
            const Point * initial_point = &points[0];
            for (size_t i = 1; i < points.size(); i++)
            {
                const Point * const next_point = &points[i];
                img.draw_line(*initial_point, *next_point, stroke);
                initial_point = next_point;
            }
        }
    }

    Line::Line(const Color &stroke,
               const Point &start,
               const Point &end)
             : Polyline(stroke, {start, end})
    {
    }
}
