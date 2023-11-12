#include "components/simple_scene.h"
#include "bits/stdc++.h"

class Enemy {
    public:
        Enemy();
        Enemy(std::string name, glm::vec3 center, float radius, glm::vec3 color, bool fill);
        ~Enemy();
        std::string name;
        glm::vec3 center;
        float radius;
        glm::vec3 color;
        bool fill;
};