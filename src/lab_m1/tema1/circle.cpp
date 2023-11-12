#include "lab_m1/tema1/circle.h"

Circle::Circle()
{
}

Circle::~Circle()
{
}

Circle::Circle(std::string name, glm::vec3 center, float radius, glm::vec3 color, bool fill)
{
    this->name = name;
    this->center = center;
    this->radius = radius;
    this->color = color;
    this->fill = fill;
}

