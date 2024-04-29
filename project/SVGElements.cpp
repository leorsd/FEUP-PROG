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
    // @todo provide the implementation of SVGElement derived classes
    // HERE -->

    // Polygon
    Polygon::Polygon(const std::vector<Point> &points, 
                     const Color &fill)
        : points(points), fill_color(fill)
    {
    }

    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points,fill_color);
    }


    // Rect
    Rect::Rect(const Point &left_top_corner, 
            const Color &fill_color, 
            const Point &width_and_height)
        : Polygon({left_top_corner, 
                left_top_corner.translate(Point{width_and_height.x, 0}), 
                left_top_corner.translate(Point{width_and_height.x, width_and_height.y}), 
                left_top_corner.translate(Point{0, width_and_height.y})}, 
                fill_color)
    {
    }

    void Rect::draw(PNGImage &img) const
    {
        img.draw_polygon(get_points(), get_fill_color());
    }
}
