#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema1/square.h"
#include "lab_m1/tema1/rectangle.h"
#include "lab_m1/tema1/circle.h"
#include "lab_m1/tema1/enemy.h"
#include <vector>

#define MAX_STARS_AVAILABLE 10
#define MAX_POINTS 10

#define ROTATION 4
#define SCALE 2
#define TRANSLATION 1

#define RED glm::vec3(0.729, 0.271, 0.239)
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

        inline glm::mat3 Translate(float translateX, float translateY)
        {
            return glm::transpose(
                    glm::mat3( 1, 0, translateX, 
                            0, 1, translateY, 
                            0, 0, 1)
            ); 
        }

        // Scale matrix
        inline glm::mat3 Scale(float scaleX, float scaleY)
        {
            return glm::transpose(
                    glm::mat3( scaleX, 0, 0, 
                            0, scaleY, 0, 
                            0, 0, 1)
            );
        }

        // Rotate matrix
        inline glm::mat3 Rotate(float radians)
        {
            return glm::transpose(
                    glm::mat3( cos(radians), -sin(radians), 0, 
                            sin(radians), cos(radians), 0, 
                            0, 0, 1)
            );
        }
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
        void SpawnEnemy();
        void HandleClickArena(int, int);
        void HandleClickPoints(int, int);
        void HandleClickShowcase(int, int);
        void HandleStillPressed(int, int);
        void HandleClickRemoveArena(int, int);
        void DestroyIfEnemiesCollide();
        template <typename T>
        void Render(vector<T>, char);
        template <typename T>
        void Render(T, char);
        void RenderWeapons();
        

        glm::vec3 position;
        GLclampf color[4];
        Mesh *actual;
        int cycle;
        std::string meshess[3];
        GLenum cullFace;
        GLenum polygonMode;
        Rectangle rectangle;
        Circle star;
        bool isPressed;
        glm::vec3 pressedColor;
        int pressedMouseX, pressedMouseY;
        Square draggedWeapon;
        vector<Square> squaresArena;
        vector<Square> squaresShowcase;
        float rombLength;
        Square basicSquare;
        
        bool is;

        vector<Square> showcaseWeapons;
        vector<Circle> usablePoints;

        vector<Enemy> enemies;
        float enemySpeed;
        
        vector<Circle> points;
        
        vector<Circle> bullets;
        vector<Square> weapons;
        
        vector<Square> lives; 
        
        float timeSinceLastSpawn;
        float spawnInterval;
        int pointsEverCreated;
        int enemiesEverCreated = 0;
        float timeSinceLastEnemy;
        float enemySpawnInterval;

    };
}   // namespace m1
