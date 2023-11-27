#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    Mesh* mesh1 = new Mesh("box1");
    mesh1->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh1->GetMeshID()] = mesh1;

    Mesh* mesh2 = new Mesh("box2");
    mesh2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh2->GetMeshID()] = mesh2;

    Mesh* mesh3 = new Mesh("tank");
    mesh3->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "personals"), "tank.obj");
    meshes[mesh3->GetMeshID()] = mesh3;
    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);
}

void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Lab4::RenderScene() {
    // modelMatrix = glm::mat4(1);
    // modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    // modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    // RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // modelMatrix = glm::mat4(1);
    // modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    // modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    // RenderMesh(meshes["box1"], shaders["Simple"], modelMatrix);

    // modelMatrix = glm::mat4(1);
    // modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    // modelMatrix *= transform3D::RotateOX(angularStepOX);
    // modelMatrix *= transform3D::RotateOY(angularStepOY);
    // modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    // RenderMesh(meshes["box2"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["tank"], shaders["VertexNormal"], modelMatrix);
}

void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    RenderScene();
    DrawCoordinateSystem();

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    // TODO(student): render the scene again, in the new viewport
    DrawCoordinateSystem();
    RenderScene();
}

void Lab4::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    int speed = 5;
    if(window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        return;
    }
    if (window->KeyHold(GLFW_KEY_A))
    {
        translateZ -= speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        translateZ += speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_W))
    {
        translateX -= speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S))
    {
        translateX += speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_R))
    {
        translateY += speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_F))
    {
        translateY -= speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_1))
    {
        scaleX -= speed * deltaTime;
        scaleY -= speed * deltaTime;
        scaleZ -= speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_2))
    {
        scaleX += speed * deltaTime;
        scaleY += speed * deltaTime;
        scaleZ += speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_3))
    {
        angularStepOX += speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_4))
    {
        angularStepOX -= speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_5))
    {
        angularStepOY += speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_6))
    {
        angularStepOY -= speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_7))
    {
        angularStepOZ += speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_8))
    {
        angularStepOZ -= speed * deltaTime;
    }

}


void Lab4::OnKeyPress(int key, int mods)
{
    int viewportSpeed = 10;
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
    if (key == GLFW_KEY_I) {
        miniViewportArea.y += viewportSpeed;
    }    
    if (key == GLFW_KEY_K) {
        miniViewportArea.y -= viewportSpeed;
    }
    if (key == GLFW_KEY_J) {
        miniViewportArea.x -= viewportSpeed;
    }
    if (key == GLFW_KEY_L) {
        miniViewportArea.x += viewportSpeed;
    }
    if (key == GLFW_KEY_O) {
        float ratio = (float)miniViewportArea.width / miniViewportArea.height;
        miniViewportArea.width += viewportSpeed * ratio;
        miniViewportArea.height += viewportSpeed;
    }
    if (key == GLFW_KEY_U) {
        float ratio = (float)miniViewportArea.width / miniViewportArea.height;
        miniViewportArea.width -= viewportSpeed * ratio;
        miniViewportArea.height -= viewportSpeed;
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
