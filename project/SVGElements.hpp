//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"
#include <string>

namespace svg
{
    class SVGElement
    {
    public:
        SVGElement();
        SVGElement(const std::string &id);
        std::string get_id() const {return id;}
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;
        virtual void translate(const Point &dir);
        virtual void rotate(const Point &origin, int degrees);
        virtual void scale(const Point &origin, int factor);
    private:
        std::string id;
    };

    // Declaration of namespace functions
    // readSVG -> implement it in readSVG.cpp
    // convert -> already given (DO NOT CHANGE) in convert.cpp

    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file,
                 const std::string &png_file);

    class Ellipse : public SVGElement
    {
    public:
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        void draw(PNGImage &img) const override;

    private:
        Color fill;
        Point center;
        Point radius;
    };
}
#endif
