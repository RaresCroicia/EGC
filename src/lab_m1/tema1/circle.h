#include "components/simple_scene.h"
#include <lab_m1/tema1/common.h>
#include "bits/stdc++.h"

class Circle : public Common {
    public:
        Circle();
        Circle(std::string name, glm::vec3 center, float radius, glm::vec3 color, bool fill);
        ~Circle();
        std::string name;
        glm::vec3 center;
        float radius;
        glm::vec3 color;
        bool fill;
        float angle;
        int cost;
        
};