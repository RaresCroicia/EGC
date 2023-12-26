#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema3/camera.h"
#include "lab_m1/tema3/transform3D.h"

#define NUMBER_OF_LIGHTSS 2

namespace m1
{
   class Tema3 : public gfxc::SimpleScene
   {
   public:
      Tema3();
      ~Tema3();

      void Init() override;

   private:
      void FrameStart() override;
      void Update(float deltaTimeSeconds) override;
      void FrameEnd() override;

      void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;
      void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *texture1);
      void RenderLighthouse(float);
      void RenderBoat(int);

      void OnInputUpdate(float deltaTime, int mods) override;
      void OnKeyPress(int key, int mods) override;
      void OnKeyRelease(int key, int mods) override;
      void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
      void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
      void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
      void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
      void OnWindowResize(int width, int height) override;

   protected:
      implemented::CameraT3 *camera;
      glm::mat4 projectionMatrix;
      bool projectionType;

      float translateX, translateY, translateZ;
      float scaleX, scaleY, scaleZ;
      float angularStepOX, angularStepOY, angularStepOZ;
      float distanceX, distanceY, distanceZ;
      float speed = 2.0f;
      float rotationSpeed = 30.0f;
      GLfloat fov, zNear, zFar;
      std::unordered_map<std::string, Texture2D *> mapTextures;
      glm::vec3 lightPosition[NUMBER_OF_LIGHTSS];
      glm::vec3 lightDirection[NUMBER_OF_LIGHTSS];
      glm::vec3 lightColor[NUMBER_OF_LIGHTSS];
      int type[NUMBER_OF_LIGHTSS];
      unsigned int materialShininess;
      float materialKd;
      float materialKs;
      float cut_off = 30;
      float angle = 0;


      // TODO(student): If you need any other class variables, define them here.
   };
} // namespace m1
