#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/camera.h"
#include "lab_m1/tema2/transform3D.h"
#include "lab_m1/tema2/bullet.h"

#include <vector>
using namespace std;
namespace m1
{
   class Tema2 : public gfxc::SimpleScene
   {
   public:
      Tema2();
      ~Tema2();

      void Init() override;

   private:
      void FrameStart() override;
      void Update(float deltaTimeSeconds) override;
      void FrameEnd() override;

      void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;
      void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix);
      void RenderBullets(float);

      void OnInputUpdate(float deltaTime, int mods) override;
      void OnKeyPress(int key, int mods) override;
      void OnKeyRelease(int key, int mods) override;
      void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
      void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
      void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
      void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
      void OnWindowResize(int width, int height) override;

   protected:
      implemented::CameraT2 *camera;
      glm::mat4 projectionMatrix;
      bool projectionType;

      float translateX, translateY, translateZ;
      float scaleX, scaleY, scaleZ;
      float angularStepOX, angularStepOY, angularStepOZ;
      float distanceX, distanceY, distanceZ;
      float speed = 2.0f;
      float rotationSpeed = 30.0f;
      GLfloat fov, zNear, zFar;
      vector<Bullet> bullets;


      // TODO(student): If you need any other class variables, define them here.
   };
} // namespace m1
