#include "lab_m1/tema1/rectangle.h"

Rectangle::Rectangle() 
{
}

Rectangle::Rectangle(std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill)
{
    this->name = name;
    this->leftBottomCorner = leftBottomCorner;
    this->width = width;
    this->height = height;
    this->color = color;
    this->fill = fill;
}

Rectangle::~Rectangle()
{
}

