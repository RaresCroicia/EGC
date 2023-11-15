#include "lab_m1/tema1/enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

Enemy::Enemy(std::string name, glm::vec3 center, float radius, glm::vec3 color1, glm::vec3 color2, bool fill)
{
    this->name = name;
    this->center = center;
    this->radius = radius;
    this->color1 = color1;
    this->color2 = color2;
    this->fill = fill;
}

