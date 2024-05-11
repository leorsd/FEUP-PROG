
#include <iostream>
#include <sstream>
#include <algorithm>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{   
    void log_svg_elements(std::vector<SVGElement*>& svg_elements, std::string base = "")
    {
        for (SVGElement* element : svg_elements)
        {
            std::cout << base << element->get_id() << std::endl;
            if (element -> is_group()) {
                Group* group = dynamic_cast<Group*>(element);
                log_svg_elements(group->get_elements(), base + "\t");
            }
        }
    }

    std::vector<SVGElement*> full_svg_elements;

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

    void processElement(XMLElement* element, vector<SVGElement *>& svg_elements)
    {
        string element_name = element->Name();
        SVGElement* svg_element = nullptr;
        const char* id_char = element->Attribute("id");
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
            string points_str = element->Attribute("points");
            replace(points_str.begin(), points_str.end(), ',', ' ');
            istringstream iss(points_str);
            vector<Point> points;
            Point p;
            while (iss >> p.x >> p.y)
            {
                points.push_back(p);
            }
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
            Point width_and_height={width-1,height-1};
            Color fill_color=parse_color(element->Attribute("fill"));
            svg_element = new Rect(top_left, width_and_height, fill_color,id);
        }
        else if (element_name == "ellipse")
        {
            Point center;
            center.x = element->IntAttribute("cx");
            center.y = element->IntAttribute("cy");
            int rx = element->IntAttribute("rx");
            int ry = element->IntAttribute("ry");
            Point radius={rx,ry};
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
            replace(points_str.begin(), points_str.end(), ',', ' ');
            istringstream iss(points_str);
            vector<Point> points;
            Point p;
            while (iss >> p.x >> p.y)
            {
                points.push_back(p);
            }
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
        else if (element_name == "g") // group element
        {
            Group* group_element = new Group({},id);
            svg_element = group_element;
            for (XMLElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
            {
                processElement(child, group_element->get_elements());
            }
        }
        else if (element_name == "use") 
        {
            string href = element->Attribute("href");
            std::cout << std::endl;
            std::cout << "Href: " << href << std::endl;
            std::cout << std::endl;
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
                    const char* origin_char = element->Attribute("transform-origin");
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
                    const char* origin_char = element->Attribute("transform-origin");
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
            full_svg_elements.push_back(svg_element);
            svg_elements.push_back(svg_element);
        }
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
        
        // TODO complete code -->
        for (XMLElement* child = xml_elem->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
        {
            processElement(child, svg_elements);
        }

        log_svg_elements(svg_elements);
    }

}