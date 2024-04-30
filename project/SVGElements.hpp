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
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        void draw(PNGImage &img) const override;

    private:
        Color fill;
        Point center;
        Point radius;
    };

    /**
     * @brief Implementation of Polygon class
     * 
     */
    class Polygon : public SVGElement
    {
    public:
        /**
         * @brief Construct a new Polygon object   
         * 
         * @param points vector of points in the polygon
         * @param fill_color color of the polygon
         */
        Polygon(const std::vector<Point> &points, const Color &fill_color);
        
        /**
         * @brief Draw the polygon on the PNG image
         * 
         * @param img destinatiton PNGImage 
         */
        void draw(PNGImage &img) const override;

        /**
         * @brief Get the points of the polygon
         * 
         * @return std::vector<Point> containing the Polygon's points
         */
        const std::vector<Point> get_points() const {return points;}

        /**
         * @brief Get the color of the polygon
         * 
         * @return Color object
         */
        const Color get_fill_color() const {return fill_color;}

    private:
        std::vector<Point> points;
        Color fill_color;
    };

    /**
     * @brief Implementation of Rectangle class(subclass of Polygon)
     * 
     */
    class Rect : public Polygon
    {
    public:
        /**
         * @brief Construct a new Rect object
         * 
         * @param left_top_corner Point representing the top left corner of the rectangle
         * @param fill_color Color of the rectangle
         * @param width_and_height Point representing the width and height of the rectangle (width, height)
         */
        Rect(const Point &left_top_corner, const Color &fill_color, const Point &width_and_height);

        /**
         * @brief Draw the rectangle on the PNG image
         * 
         * @param img destination PNGImage
         */
        void draw(PNGImage &img) const override;
    };
}
#endif
