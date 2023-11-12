#include "lab_m1/tema1/enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

Enemy::Enemy(std::string name, glm::vec3 center, float radius, glm::vec3 color, bool fill)
{
    this->name = name;
    this->center = center;
    this->radius = radius;
    this->color = color;
    this->fill = fill;
}

