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

    /**
     * @brief Implementation of the Ellipse class
     * 
     */
    class Ellipse : public SVGElement
    {
    public:
        /**
         * @brief Construct a new Ellipse object
         * 
         * @param fill color of the ellipse
         * @param center coordinates of the center of the ellipse
         * @param radius radius of the ellipse (X-axis and Y-axis)
         */
        Ellipse(const Color &fill, const Point &center, const Point &radius);

        /**
         * @brief Draw the ellipse on the PNG image
         * 
         * @param img destination PNG image
         */
        void draw(PNGImage &img) const override;

    private:
        Color fill;
        Point center;
        Point radius;
    };

    /**
     * @brief Implementation of the Polyline class
     * 
     */
    class Polyline : public SVGElement
    {
    public:
        /**
         * @brief Construct a new Polyline object
         * 
         * @param stroke color of the polyline
         * @param points vector of points in the polyline
         */
        Polyline(const Color &stroke, const std::vector<Point> &points);

        /**
         * @brief Draw the polyline on the PNG image
         * 
         * @param img destination PNG image
         */
        void draw(PNGImage &img) const override;

        /**
         * @brief Get the points of the polyline
         * 
         * @return std::vector<Point> containing the Polyline's Points
         */
        const std::vector<Point>& get_points() const { return points; }

        /**
         * @brief Get the Color of the Polyline
         * 
         * @return Color object
         */
        const Color& get_stroke() const { return stroke; }

    private:
        Color stroke;
        std::vector<Point> points;
    };

    /**
     * @brief Implementation of the Line class (subclass of Polyline)
     * 
     */
    class Line : public Polyline
    {
    public:

        /**
         * @brief Construct a new Line object
         * 
         * @param stroke color of the line
         * @param start starting point of the line (XY coordinates)
         * @param end ending point of the line (XY coordinates)
         */
        Line(const Color &stroke, const Point &start, const Point &end);

        /**
         * @brief Draw the Line on the PNG image
         * 
         * @param img destination PNG image
         */
        void draw(PNGImage &img) const override;

        /**
         * @brief Get the initial Point
         * 
         * @return First Point in the vector
         */
        const Point get_initial_point() const { return *(get_points().begin()); }

        /**
         * @brief Get the final Point
         * 
         * @return Last Point in the vector
         */
        const Point get_final_point() const { return get_points().back(); }
    };
}
#endif
