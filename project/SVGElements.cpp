#include "SVGElements.hpp"
namespace svg
{   
    // SVGElement
    SVGElement::SVGElement(): fill(Color{0,0,0}), id("undefined") {}

    SVGElement::SVGElement(const Color &fill, const std::string &id): fill(fill), id(id) {} 

    SVGElement::~SVGElement() {}

    Ellipse::Ellipse(const Point &center, const Point &radius,
                     const Color &fill, const std::string &id)
                : SVGElement(fill, id), center(center), radius(radius)
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

    void Ellipse::translate(const Point &dir)
    {
        center = center.translate(dir);
    }

    void Ellipse::rotate(const Point &origin, int degrees)
    {
        center = center.rotate(origin, degrees);
    }

    void Ellipse::scale(const Point &origin, int factor)
    {
        center = center.scale(origin, factor);
        radius = radius.scale(Point{0,0}, factor);
    }

    // Circle
    Circle::Circle(const Point &center,
                   int radius,
                   const Color &fill, 
                   const std::string &id)
        : Ellipse(center, Point{radius, radius}, fill, id)
    {
    }

    Polyline::Polyline(const std::vector<Point> &points,
                       const Color &stroke,
                       const std::string &id )
                    :  SVGElement(stroke, id), points(points)
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
                img.draw_line(*initial_point, *next_point, fill);
                initial_point = next_point;
            }
        }
    }

    void Polyline::translate(const Point &dir)
    {
        for (Point &p:points)
        {
            p=p.translate(dir);
        }
    }

    void Polyline::rotate(const Point &origin, int degrees)
    {
        for (Point &p:points)
        {
            p=p.rotate(origin, degrees);
        }
    }

    void Polyline::scale(const Point &origin, int factor)
    {
        for (Point &p:points)
        {
            p=p.scale(origin, factor);
        }
    }

    // Line
    Line::Line(const Point &start,
               const Point &end,
               const Color &stroke,
               const std::string &id)
             : Polyline({start, end}, stroke, id)
    {
    }

    // Polygon
    Polygon::Polygon(const std::vector<Point> &points, 
                     const Color &fill,
                     const std::string &id)
        : SVGElement(fill, id), points(points)
    {
            std::cout<<points.size()<<std::endl;
    }

    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points,fill);
    }

    void Polygon::translate(const Point &dir)
    {
        for (Point &p:points)
        {
            p=p.translate(dir);
        }
    }

    void Polygon::rotate(const Point &origin, int degrees)
    {
        for (Point &p:points)
        {
            p=p.rotate(origin, degrees);
        }
    }

    void Polygon::scale(const Point &origin, int factor)
    {
        for (Point &p:points)
        {
            p=p.scale(origin, factor);
        }
    }


    // Rect
    Rect::Rect(const Point &left_top_corner, 
            const Point &width_and_height,
            const Color &fill_color,
            const std::string &id)
        : Polygon({left_top_corner, 
                left_top_corner.translate(Point{width_and_height.x, 0}), 
                left_top_corner.translate(Point{width_and_height.x, width_and_height.y}), 
                left_top_corner.translate(Point{0, width_and_height.y})}, 
                fill_color, id),
        width_and_height(width_and_height)
    {
    }

    Group::Group(const std::vector<SVGElement*> &elements, const std::string &id)
        : SVGElement(Color{0,0,0}, id), elements(elements)
    {
    }

    Group::~Group()
    {
        for (SVGElement *element: elements)
        {
            delete element;
        }
    }

    void Group::draw(PNGImage &img) const {
        for (const SVGElement *element: elements)
        {
            element->draw(img);
        }
    }

    void Group::translate(const Point &dir) {
        for (SVGElement *element: elements)
        {
            element->translate(dir);
        }
    }

    void Group::rotate(const Point &origin, int degrees) {
        for (SVGElement *element: elements)
        {
            element->rotate(origin, degrees);
        }
    }

    void Group::scale(const Point &origin, int factor) {
        for (SVGElement *element: elements)
        {
            element->scale(origin, factor);
        }
    }

    void Group::add_element(SVGElement *element) {
        elements.push_back(element);
    }
}
