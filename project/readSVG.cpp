
#include <iostream>
#include <sstream>
#include <algorithm>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{
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
        
        // TODO complete code -->
        for (XMLElement* child = xml_elem->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
        {
            string element_name = child->Name();
            SVGElement* svg_element = nullptr;
            const char* id_char = child->Attribute("id");
            string id;
            if (id_char!=NULL) 
            {
                id = id_char;
            } else 
            {
                id = "undefined";
            }
            if (element_name == "polygon")
            {
                string points_str = child->Attribute("points");
                replace(points_str.begin(), points_str.end(), ',', ' ');
                istringstream iss(points_str);
                vector<Point> points;
                Point p;
                while (iss >> p.x >> p.y)
                {
                    points.push_back(p);
                }
                Color fill_color=parse_color(child->Attribute("fill"));
                svg_element = new Polygon(points,fill_color,id);
            }
            else if (element_name == "rect")
            {
                Point top_left;
                top_left.x = child->IntAttribute("x");
                top_left.y = child->IntAttribute("y");
                int width = child->IntAttribute("width");
                int height = child->IntAttribute("height");
                Point width_and_height={width-1,height-1};
                Color fill_color=parse_color(child->Attribute("fill"));
                svg_element = new Rect(top_left, width_and_height, fill_color,id);
            }
            else if (element_name == "ellipse")
            {
                Point center;
                center.x = child->IntAttribute("cx");
                center.y = child->IntAttribute("cy");
                int rx = child->IntAttribute("rx");
                int ry = child->IntAttribute("ry");
                Point radius={rx,ry};
                Color fill_color=parse_color(child->Attribute("fill"));
                svg_element = new Ellipse(center, radius, fill_color, id);
            }
            else if (element_name == "circle")
            {
                Point center;
                center.x = child->IntAttribute("cx");
                center.y = child->IntAttribute("cy");
                int radius = child->IntAttribute("r");
                Color fill_color=parse_color(child->Attribute("fill"));
                svg_element = new Circle(center, radius,fill_color,id);
            }
            else if (element_name == "polyline")
            {
                string points_str = child->Attribute("points");
                replace(points_str.begin(), points_str.end(), ',', ' ');
                istringstream iss(points_str);
                vector<Point> points;
                Point p;
                while (iss >> p.x >> p.y)
                {
                    points.push_back(p);
                }
                Color fill_color=parse_color(child->Attribute("stroke"));
                svg_element = new Polyline(points,fill_color,id);
            }
            else if (element_name == "line")
            {
                Point start;
                start.x = child->IntAttribute("x1");
                start.y = child->IntAttribute("y1");
                Point end;
                end.x = child->IntAttribute("x2");
                end.y = child->IntAttribute("y2");
                Color fill_color=parse_color(child->Attribute("stroke"));
                svg_element = new Line(start, end,fill_color,id);
            }
            if (svg_element != nullptr)
            {
                const char* transform_char = child->Attribute("transform");
                if (transform_char != NULL)
                {
                    string transform_str = transform_char;
                    if (transform_str.find("translate") != string::npos)
                    {
                        size_t start = transform_str.find("(") + 1;
                        size_t end = transform_str.find(")");
                        string values_str = transform_str.substr(start, end - start);
                        replace(values_str.begin(), values_str.end(), ',', ' ');
                        istringstream iss(values_str);
                        Point translation;
                        iss >> translation.x >> translation.y;
                        svg_element->translate(translation);
                    }
                    else if (transform_str.find("rotate") != string::npos)
                    {
                        size_t start = transform_str.find("(") + 1;
                        size_t end = transform_str.find(")");
                        string angle_str = transform_str.substr(start, end - start);
                        int angle = stoi(angle_str);
                        const char* origin_char = child->Attribute("transform-origin");
                        Point origin;
                        if (origin_char != NULL)
                        {
                            string origin_str = origin_char;
                            replace(origin_str.begin(), origin_str.end(), ',', ' ');
                            istringstream iss(origin_str);
                            iss >> origin.x >> origin.y;
                        }else {
                            origin.x = 0;
                            origin.y = 0;
                        }
                        svg_element->rotate(origin, angle);
                    }
                    else if (transform_str.find("scale") != string::npos)
                    {
                        size_t start = transform_str.find("(") + 1;
                        size_t end = transform_str.find(")");
                        string value_str = transform_str.substr(start, end - start);
                        int scale=stoi(value_str);
                        const char* origin_char = child->Attribute("transform-origin");
                        Point origin;
                        if (origin_char != NULL)
                        {
                            string origin_str = origin_char;
                            replace(origin_str.begin(), origin_str.end(), ',', ' ');
                            istringstream iss(origin_str);
                            iss >> origin.x >> origin.y;
                        }else {
                            origin.x = 0;
                            origin.y = 0;
                        }
                        svg_element->scale(origin, scale);
                    }

                }
                svg_elements.push_back(svg_element);
            }
        }
        
    }
}