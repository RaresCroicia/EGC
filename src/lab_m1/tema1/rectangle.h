#include <bits/stdc++.h>
#include "components/simple_scene.h"

class Rectangle {
    public:
        Rectangle();
        Rectangle(std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill);
        ~Rectangle();
        std::string name;
        glm::vec3 leftBottomCorner;
        float width, height;
        glm::vec3 color;
        bool fill;
        glm::vec3 center;
        float angle;
};