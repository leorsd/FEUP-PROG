
#include <iostream>
#include <sstream>
#include <algorithm>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{   
    /* global variable to store all the elements*/
    std::vector<SVGElement*> full_svg_elements;

    /** 
     * @brief get the type of the element by its id
     * @param id the id of the element
    */
    SVGElement * get_element_by_id(const string& id)
    {
        for (SVGElement * element : full_svg_elements)
        {
            if (element->get_id() == id)
            {
                return element;
            }
        }
        return nullptr;
    }
    /**
     * @brief get a vector of points from a string
     * used in ProcessElement (polygon, polyline)
     * 
     * @param points_str 
     * @return vector<Point> 
     */
    vector<Point> string_to_vector_of_points(string points_str)
    {
        replace(points_str.begin(), points_str.end(), ',', ' ');
        istringstream iss(points_str);
        vector<Point> points;
        Point p;
        while (iss >> p.x >> p.y)
        {
            points.push_back(p);
        }
        return points;
    }

    /**
     * @brief get a point from a string
     * used in ProcessElement (translate, rotate, scale)
     * 
     * @param point_str 
     * @return Point 
     */
    Point string_to_point(string point_str)
    {
        size_t start = point_str.find("(") + 1;
        size_t end = point_str.find(")");
        string values_str = point_str.substr(start, end - start);
        replace(values_str.begin(), values_str.end(), ',', ' ');
        istringstream iss(values_str);
        Point translation;
        iss >> translation.x >> translation.y;
        return translation;
    }

    /**
     * @brief convert a string with brackets to an integer
     * used in ProcessElement (rotate, scale)
     * 
     * @param int_str 
     * @return int 
     */
    int string_to_int(string int_str)
    {
        size_t start = int_str.find("(") + 1;
        size_t end = int_str.find(")");
        string angle_str = int_str.substr(start, end - start);
        int value = stoi(angle_str);
        return value;
    }

    void process_element(XMLElement* element, vector<SVGElement *>& svg_elements)
    {
        string element_name = element->Name();
        /* pointer to the element to be created; creating the element outside the if statements saves some lines */
        SVGElement* svg_element = nullptr;
        const char* id_char = element->Attribute("id");    
        string id;
        /* If the element doesn't have an id, id_char will be NULL */
        if (id_char!=NULL)
        {
            id = id_char;
        } else /* Default id is "undefined" */
        {
            id = "undefined";
        }
        if (element_name == "polygon")
        {
            string points_str = element->Attribute("points");
            vector<Point> points=string_to_vector_of_points(points_str);
            Color fill_color=parse_color(element->Attribute("fill"));
            svg_element = new Polygon(points,fill_color,id);
        }
        else if (element_name == "rect")
        {
            Point top_left;
            top_left.x = element->IntAttribute("x");
            top_left.y = element->IntAttribute("y");
            int width = element->IntAttribute("width");
            int height = element->IntAttribute("height");
            Point width_and_height={width-1,height-1};      /* -1 because the width and height begin in 0 */
            Color fill_color=parse_color(element->Attribute("fill"));
            svg_element = new Rect(top_left, width_and_height, fill_color,id);
        }
        else if (element_name == "ellipse")
        {
            Point center;
            center.x = element->IntAttribute("cx");
            center.y = element->IntAttribute("cy");
            Point radius;
            radius.x = element->IntAttribute("rx");
            radius.y = element->IntAttribute("ry");
            Color fill_color=parse_color(element->Attribute("fill"));
            svg_element = new Ellipse(center, radius, fill_color, id);
        }
        else if (element_name == "circle")
        {
            Point center;
            center.x = element->IntAttribute("cx");
            center.y = element->IntAttribute("cy");
            int radius = element->IntAttribute("r");
            Color fill_color=parse_color(element->Attribute("fill"));
            svg_element = new Circle(center, radius,fill_color,id);
        }
        else if (element_name == "polyline")
        {
            string points_str = element->Attribute("points");
            vector<Point> points=string_to_vector_of_points(points_str);
            Color fill_color=parse_color(element->Attribute("stroke"));
            svg_element = new Polyline(points,fill_color,id);
        }
        else if (element_name == "line")
        {
            Point start;
            start.x = element->IntAttribute("x1");
            start.y = element->IntAttribute("y1");
            Point end;
            end.x = element->IntAttribute("x2");
            end.y = element->IntAttribute("y2");
            Color fill_color=parse_color(element->Attribute("stroke"));
            svg_element = new Line(start, end,fill_color,id);
        }
        else if (element_name == "g") /* group element */
        {
            Group* group_element = new Group({},id);
            svg_element = group_element;
            for (XMLElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
            {
                process_element(child, group_element->get_elements()); /* recursive call in case of nested groups  */
            }
        }
        else if (element_name == "use") 
        {
            string href = element->Attribute("href");
            string old_id = href.substr(1);
            SVGElement* referenced_element = get_element_by_id(old_id);
            if (referenced_element != nullptr)
            {
                svg_element = referenced_element->clone(id);
            }
        }

        if (svg_element != nullptr)
        {
            const char* transform_char = element->Attribute("transform");
            if (transform_char != NULL)
            {
                string transform_str = transform_char;
                if (transform_str.find("translate") != string::npos)
                {
                    Point translation = string_to_point(transform_str);
                    svg_element->translate(translation);
                }
                else if (transform_str.find("rotate") != string::npos)
                {
                    int angle = string_to_int(transform_str);
                    const char* origin_char = element->Attribute("transform-origin");
                    Point origin;
                    if (origin_char != NULL)
                    {
                        string origin_str = origin_char;
                        origin = string_to_point(origin_str);
                    }else {
                        origin.x = 0;
                        origin.y = 0;
                    }
                    svg_element->rotate(origin, angle);
                }
                else if (transform_str.find("scale") != string::npos)
                {
                    int scale=string_to_int(transform_str);
                    const char* origin_char = element->Attribute("transform-origin");
                    Point origin;
                    if (origin_char != NULL)
                    {
                        string origin_str = origin_char;
                        origin = string_to_point(origin_str);
                    }else {
                        origin.x = 0;
                        origin.y = 0;
                    }
                    svg_element->scale(origin, scale);
                }
            }    
        }
        full_svg_elements.push_back(svg_element);       
        svg_elements.push_back(svg_element); 
    }

    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
    {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Unable to load " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();

        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");
        
        for (XMLElement* child = xml_elem->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
        {
            process_element(child, svg_elements);
        }
    }

}
