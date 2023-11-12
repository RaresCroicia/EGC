#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema1/square.h"
#include "lab_m1/tema1/rectangle.h"
#include "lab_m1/tema1/circle.h"
#include "lab_m1/tema1/enemy.h"
#include <vector>

#define MAX_STARS_AVAILABLE 10
#define MAX_POINTS 10

#define RED glm::vec3(1, 0, 0)
#define GREEN glm::vec3(0, 1, 0)
#define BLUE glm::vec3(0, 0, 1)
#define YELLOW glm::vec3(1, 1, 0)
#define CYAN glm::vec3(0, 1, 1)
#define MAGENTA glm::vec3(1, 0, 1)
#define WHITE glm::vec3(1, 1, 1)
#define BLACK glm::vec3(0, 0, 0)
#define ORANGE glm::vec3(1, 0.5, 0)

using namespace std;
namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
        Mesh* CreateRectangle(const std::string &name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill);
        Mesh* CreateStar(const std::string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill);
        Mesh* CreateRomb(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill);
        Mesh* CreateEnemy(const std::string &name, glm::vec3 center, float radius, glm::vec3 color, glm::vec3 color2, bool fill);
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void CreateScene(glm::ivec2 resolution);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void SpawnPoint();
        bool AddPoint();
        bool RemovePoint();

        glm::vec3 position;
        GLclampf color[4];
        Mesh *actual;
        int cycle;
        std::string meshess[3];
        GLenum cullFace;
        GLenum polygonMode;
        vector<Square> squares;
        Rectangle rectangle;
        Circle star;

        vector<Enemy> enemies;
        float enemySpeed;
        vector<Circle> points;
        vector<Circle> bullets;
        vector<Square> weapons;
        vector<Square> lives; 
        vector<Square> showcaseWeapons;
        vector<Circle> usablePoints;
        float timeSinceLastSpawn;
        float spawnInterval;
        int pointsEverCreated;

    };
}   // namespace m1
