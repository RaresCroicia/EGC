#include "components/simple_scene.h"
#include <lab_m1/tema1/common.h>
#include "bits/stdc++.h"

class Enemy : public Common{
    public:
        Enemy();
        Enemy(std::string name, glm::vec3 center, float radius, glm::vec3 color1, glm::vec3 color2, bool fill);
        ~Enemy();
        std::string name;
        glm::vec3 center;
        float radius;
        glm::vec3 color1, color2;
        bool fill;
        float angle;
        bool isToDelete = false;
};