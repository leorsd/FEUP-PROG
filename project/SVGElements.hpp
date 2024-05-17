//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"
#include <string>
#include <iostream>

namespace svg
{
    /**
     * @brief Declaration of the SVGElement class
     * 
     */
    class SVGElement
    {
    public:
        /**
         * @brief Construct a new SVGElement object
         * 
         */
        SVGElement();

        /**
         * @brief Construct a new SVGElement object
         * 
         * @param fill color of the SVGElement
         * @param id string representing the id of the SVGElement
         */
        SVGElement(const Color &fill, const std::string &id);

        /**
         * @brief Get the id of the SVGElement
         * 
         * @return string containing the id of the SVGElement
         */
        std::string get_id() const {return id;}

        /**
         * @brief Destroy the SVGElement object
         * 
         */
        virtual ~SVGElement();

        /**
         * @brief Clone the SVGElement
         * 
         * @return SVGElement* 
         */
        virtual SVGElement* clone(const std::string &id) const = 0;

        /**
         * @brief Draw the SVGElement on the PNG image
         * 
         * @param img destination PNG image
         */
        virtual void draw(PNGImage &img) const = 0;

        /**
         * @brief Translate the SVGElement
         * 
         * @param dir Point representing the X and Y axes units of the translation (x,y)
         */
        virtual void translate(const Point &dir) = 0;

        /**
         * @brief Rotate the SVGElement
         * 
         * @param origin Point representing the origin of the rotation
         * @param degrees int representing the degrees of the rotation
         */
        virtual void rotate(const Point &origin, int degrees) = 0;

        /**
         * @brief Scale the SVGElement
         * 
         * @param origin Point representing the origin of the scaling
         * @param factor int representing the factor of the scaling
         */
        virtual void scale(const Point &origin, int factor) = 0;

    protected:
        Color fill;
        std::string id;
    };


    /**
     * @brief Declaration of the Ellipse class
     * 
     */
    class Ellipse : public SVGElement
    {
    public:

        /**
         * @brief Construct a new Ellipse object
         * 
         * @param center coordinates of the center of the ellipse
         * @param radius radius of the ellipse (X-axis and Y-axis)
         * @param fill color of the ellipse
         * @param id string representing the id of the ellipse
         */
        Ellipse(const Point &center, const Point &radius, 
        const Color &fill, const std::string &id);

        /**
         * @brief Clone the ellipse
         * 
         * @return Ellipse* 
         */
        Ellipse* clone(const std::string &id) const override;

        /**
         * @brief Draw the ellipse on the PNG image
         * 
         * @param img destination PNG image
         */
        void draw(PNGImage &img) const override;

        /**
         * @brief Translate the ellipse
         * 
         * @param dir Point representing the X and Y axes units of the translation (x,y)
         */
        void translate(const Point &dir) override;

        /**
         * @brief Rotate the ellipse
         * 
         * @param origin Point representing the origin of the rotation
         * @param degrees Integer representing the degrees of the rotation
         */
        void rotate(const Point &origin, int degrees) override;

        /**
         * @brief Scale the ellipse
         * 
         * @param origin Point representing the origin of the scaling
         * @param factor Integer representing the factor of the scaling
         */
        void scale(const Point &origin, int factor) override;

    protected:
        Point center;
        Point radius;
    };
  
    /**
     * @brief Declaration of the Circle class (subclass of Ellipse)
     * 
     */
    class Circle final : public Ellipse
    {
    public:

        /**
         * @brief Circle object constructor
         * 
         * @param center coordinates of the circle's center
         * @param radius of the circle
         * @param fill color of the circle
         * @param id string representing the id of the circle
         */
        Circle(const Point &center, int radius, const Color &fill, const std::string &id);
    };


    /**
     * @brief Declaration of the Polyline class
     * 
     */
    class Polyline : public SVGElement
    {
    public:

        /**
         * @brief Construct a new Polyline object
         * 
         * @param points vector of points in the polyline
         * @param stroke color of the polyline
         * @param id string representing the id of the polyline
         */
        Polyline(const std::vector<Point> &points, const Color &stroke, const std::string &id);

        /**
         * @brief Clone the polyline
         * 
         * @return Polyline* 
         */
        Polyline* clone(const std::string &id) const override;

        /**
         * @brief Draw the polyline on the PNG image
         * 
         * @param img destination PNG image
         */
        void draw(PNGImage &img) const override;

        /**
         * @brief Translate the polyline
         * 
         * @param dir Point representing the X and Y axes units of the translation (x,y)
         */
        void translate(const Point &dir) override;

        /**
         * @brief Rotate the polyline
         * 
         * @param origin Point representing the origin of the rotation
         * @param degrees Integer representing the degrees of the rotation
         */
        void rotate(const Point &origin, int degrees) override;

        /**
         * @brief Scale the polyline
         * 
         * @param origin Point representing the origin of the scaling
         * @param factor Integer representing the factor of the scaling
         */
        void scale(const Point &origin, int factor) override;

    protected:
        std::vector<Point> points;
    };

    /**
     * @brief Declaration of the Line class (subclass of Polyline)
     * 
     */
    class Line final : public Polyline
    {
    public:

        /**
         * @brief Construct a new Line object
         * 
         * @param start starting point of the line (XY coordinates)
         * @param end ending point of the line (XY coordinates)
         * @param stroke color of the line
         * @param id string representing the id of the line
         */
        Line(const Point &start, const Point &end, const Color &stroke, const std::string &id);
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
         * @param id string representing the id of the polygon
         */
        Polygon(const std::vector<Point> &points, const Color &fill_color, const std::string &id);
        
        /**
         * @brief Clone the polygon
         * 
         * @return Polygon* 
         */
        Polygon* clone(const std::string &id) const override;

        /**
         * @brief Draw the polygon on the PNG image
         * 
         * @param img destinatiton PNGImage 
         */
        void draw(PNGImage &img) const override;

        /**
         * @brief Translate the polygon
         * 
         * @param dir Point representing the X and Y axes units of the translation (x,y)
         */
        void translate(const Point &dir) override;

        /**
         * @brief Rotate the polygon
         * 
         * @param origin Point representing the origin of the rotation
         * @param degrees Integer representing the degrees of the rotation
         */
        void rotate(const Point &origin, int degrees) override;

        /**
         * @brief Scale the polygon
         * 
         * @param origin Point representing the origin of the scaling
         * @param factor Integer representing the factor of the scaling
         */
        void scale(const Point &origin, int factor) override;

    protected:
        std::vector<Point> points;
    };

    /**
     * @brief Implementation of Rectangle class (subclass of Polygon)
     * 
     */
    class Rect final : public Polygon
    {
    public:
        /**
         * @brief Construct a new Rect object
         * 
         * @param left_top_corner Point representing the top left corner of the rectangle
         * @param width_and_height Point representing the width and height of the rectangle (width, height)
         * @param fill_color Color of the rectangle
         * @param id string representing the id of the rectangle
         */
        Rect(const Point &left_top_corner, const Point &width_and_height, const Color &fill_color, const std::string &id);
    };

    /**
     * @brief Declaration of the Group class
     * 
     */
    class Group : public SVGElement
    {
        public : 
            /**
             * @brief Construct a new Group object
             * 
             * @param elements vector og SVGElements
             * @param id string representing the id of the group
             */
            Group(const std::vector<SVGElement *> &elements = {}, const std::string &id="undefined");

            /**
             * @brief Destroy the Group object
             * 
             */
            ~Group();

            /**
             * @brief Clone the group
             * 
             * @return Group* 
             */
            Group* clone(const std::string &id) const override;

            /**
             * @brief Draw all elements in the group on the PNG image
             * 
             * @param img destination PNG image
             */
            void draw(PNGImage &img) const override;

            /**
             * @brief Translate all elements in the group
             * 
             * @param dir Point representing the X and Y coordinates of the translation (x,y)
             */
            void translate(const Point &dir) override;

            /**
             * @brief Rotate all elements in the group
             * 
             * @param origin Point representing the origin of the rotation
             * @param degrees Integer representing the degrees of the rotation
             */
            void rotate(const Point &origin, int degrees) override;

            /**
             * @brief Scale all elements in the group
             * 
             * @param origin Point representing the origin of the scaling
             * @param factor Integer representing the factor of the scaling
             */
            void scale(const Point &origin, int factor) override;

            /**
             * @brief Add an element to the group
             * 
             * @param element SVGElement to be added to the group
             */
            void add_element(SVGElement *element);

            /**
             * @brief Get the elements of the group
             * 
             * @return std::vector<SVGElement *> containing the elements of the group
             */
            std::vector<SVGElement *>& get_elements() {return elements;}
        private:
            std::vector<SVGElement *> elements;
    };

    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<svg::SVGElement *> &svg_elements);

    void convert(const std::string &svg_file,
                 const std::string &png_file);
}
#endif
