#include <bits/stdc++.h>
#include "components/simple_scene.h"

class Square {
    public:
        Square();
        Square(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill);
        ~Square();
        std::string name;
        glm::vec3 leftBottomCorner;
        float length;
        glm::vec3 color;
        bool fill;
};