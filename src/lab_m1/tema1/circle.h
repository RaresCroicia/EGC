#include "components/simple_scene.h"
#include "bits/stdc++.h"

class Circle {
    public:
        Circle();
        Circle(std::string name, glm::vec3 center, float radius, glm::vec3 color, bool fill);
        ~Circle();
        std::string name;
        glm::vec3 center;
        float radius;
        glm::vec3 color;
        bool fill;
};