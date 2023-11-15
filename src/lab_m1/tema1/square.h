#include <bits/stdc++.h>
#include "components/simple_scene.h"

class Square {
    public:
        Square();
        Square(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill);
        ~Square();
        std::string name;
        int id;
        glm::vec3 leftBottomCorner;
        float length;
        glm::vec3 color;
        bool fill;
        glm::vec3 center;
        float angle;
        glm::vec3 colorInside = glm::vec3(0, 0, 0);
};