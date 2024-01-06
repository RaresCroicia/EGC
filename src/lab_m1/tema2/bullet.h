#ifndef __BULLET_H__
#define __BULLET_H__

#include <components/simple_scene.h>

class Bullet {
    public:
        Bullet();
        ~Bullet();
        glm::vec3 position;
        float angle;
        int frames = 0;
};

#endif 