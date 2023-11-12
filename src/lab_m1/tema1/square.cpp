#include "lab_m1/tema1/square.h"

Square::Square() 
{
}

Square::Square(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
    this->name = name;
    this->leftBottomCorner = leftBottomCorner;
    this->length = length;
    this->color = color;
    this->fill = fill;
}

Square::~Square()
{
}