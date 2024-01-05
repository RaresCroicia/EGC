#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema3/transform3D.h"
#include "lab_m1/tema3/camera.h"
// #include "components/transform.h"
#define NUMBER_OF_BOATS 4

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

      void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *texture1, int isBoat = false);
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
      float translateX, translateY, translateZ;
      float scaleX, scaleY, scaleZ;
      float angularStepOX, angularStepOY, angularStepOZ;
      float distanceX, distanceY, distanceZ;
      float speed = 2.0f;
      float rotationSpeed = 30.0f;
      GLfloat fov = 60.f, zNear = 0.01, zFar = 200.f;
      glm::mat4 projectionMatrix;
      implemented::CameraT3 *camera;
      std::unordered_map<std::string, Texture2D *> mapTextures;
      glm::vec3 lightPosition[NUMBER_OF_BOATS];
      glm::vec3 lightDirection[NUMBER_OF_BOATS];
      glm::vec3 lightColor[NUMBER_OF_BOATS];
      glm::vec3 becLightPos;
      glm::vec3 becLightDir;
      glm::vec3 becLightColor;
      glm::vec3 spotFarPos1;
      glm::vec3 spotFarDir1;
      glm::vec3 spotFarColor1;
      glm::vec3 spotFarPos2;
      glm::vec3 spotFarDir2;
      glm::vec3 spotFarColor2;
      glm::vec3 moonLightPos;
      glm::vec3 moonLightDir;
      glm::vec3 moonLightColor;
      unsigned int materialShininess;
      float materialKd;
      float materialKs;
      float cut_off = 30;
      float angle = 0;
      float randomY = 0;
      float boatAngles[NUMBER_OF_BOATS];
      float boatDistances[NUMBER_OF_BOATS];
      float boatDirections[NUMBER_OF_BOATS];
      float boatSpeeds[NUMBER_OF_BOATS];
      // TODO(student): If you need any other class variables, define them here.
   };
} // namespace m1
