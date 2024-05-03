//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"

namespace svg
{
    class SVGElement
    {

    public:
        SVGElement();
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;
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
        /**
         * @brief Ellipse object constructor
         * 
         * @param fill 
         * @param center 
         * @param radius 
         */
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        /**
         * @brief Get the center
         * 
         * @return Point 
         */
        Point get_center() const;
        /**
         * @brief Get the radius
         * 
         * @return Point 
         */
        Point get_radius() const;
        /**
         * @brief draw the Ellipse
         * 
         * @param img 
         */
        void draw(PNGImage &img) const override;

    private:
        Color fill;
        Point center;
        Point radius;
    };

    Class Circle : public Ellipse
    {
    public:
        /**
         * @brief Circle object constructor
         * 
         * @param fill 
         * @param center 
         * @param radius 
         */
        Circle(const Color &fill, const Point &center, double radius);
        /**
         * @brief Get the radius
         * 
         * @return double 
         */
        double get_radius() const;
        /**
         * @brief draw the Circle
         * 
         * @param img 
         */
        void draw(PNGImage &img) const override;
    };
}
#endif
