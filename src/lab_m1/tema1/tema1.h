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
        // 2D Transformations
        inline glm::mat3 Translate(float translateX, float translateY)
        {
            return glm::transpose(
                    glm::mat3( 1, 0, translateX, 
                            0, 1, translateY, 
                            0, 0, 1)
            ); 
        }

        inline glm::mat3 Scale(float scaleX, float scaleY)
        {
            return glm::transpose(
                    glm::mat3( scaleX, 0, 0, 
                            0, scaleY, 0, 
                            0, 0, 1)
            );
        }

        inline glm::mat3 Rotate(float radians)
        {
            return glm::transpose(
                    glm::mat3( cos(radians), -sin(radians), 0, 
                            sin(radians), cos(radians), 0, 
                            0, 0, 1)
            );
        }

        // CREATE MESHES
        Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
        Mesh* CreateRectangle(const std::string &name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill);
        Mesh* CreateStar(const std::string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill);
        Mesh* CreateRomb(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill);
        Mesh* CreateEnemy(const std::string &name, glm::vec3 center, float radius, glm::vec3 color, glm::vec3 color2, bool fill);

        // SCENE CREATION AND RENDERING
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void CreateScene(glm::ivec2 resolution);

        // EVENTS
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        // SPAWNERS
        void SpawnPoint();
        void SpawnEnemy();
        bool SpawnBullet();

        // POINT HANDLERS
        bool AddPoint();
        bool RemovePoint(int);

        // EVENT HANDLERS
        void HandleClickPoints(int, int);
        void HandleClickShowcase(int, int);
        void HandleStillPressed(int, int);
        void HandleClickRemoveArena(int, int);
        void HandleRelease(int, int);

        // COLLIDE HANDLERS
        void DestroyIfEnemiesCollide();
        void DamageIfBulletCollides();

        // RENDERERS
        template <typename T>
        void Render(vector<T>, char);

        template <typename T>
        void Render(T, char);

        void RenderWeapons();
        void RenderBullets(float);
        void RenderEnemies();

        // AUXILIARY FUNCTIONS
        string GetColor(glm::vec3);
        
        // DRAG & DROP VARIABLES
        string pickedColor = "nothing";
        bool isPressed;

        // GAME VARIABLES
        GLclampf color[4];
        Rectangle rectangle;
        Square draggedWeapon;

        // SHAPE VARIABLES
        float rombLength;
        Square basicSquare;
        
        // GAME STATE VARIABLES
        bool isGameOver = false;

        // GAME OBJECTS
        vector<Square> squaresArena;
        vector<Square> squaresShowcase;
        vector<Circle> showcasePoints;
        vector<Square> showcaseWeapons;
        vector<Circle> usablePoints;
        vector<Enemy> enemies;
        vector<Circle> points;
        vector<Circle> bullets;
        vector<Square> weapons;
        vector<Square> lives; 
        unordered_map<string, int> colorsCost;

        // GAME VARIABLES
        float enemySpeed;
        float bulletSpeed;

        float timeSinceLastSpawn;
        float spawnInterval;
        float timeSinceLastEnemy;
        float enemySpawnInterval;
        float timeSinceLastBullet;
        float bulletSpawnInterval;

        int pointsEverCreated;
        int enemiesEverCreated = 0;
        int bulletsEverCreated = 0;
        
        int chanceSpawnEnemy;
        int chanceSpawnPoint;

    };
}