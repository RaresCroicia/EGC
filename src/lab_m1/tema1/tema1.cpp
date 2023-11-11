#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
    // TODO(student): Never forget to initialize class variables!

}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    position = glm::vec3(0, 0, 0);
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    color[3] = 1;
        
    meshess[0] = "iepure"; 
    meshess[1] = "sfera"; 
    meshess[2] = "ceasca";

    {
        Mesh* mesh = new Mesh("iepure");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "animals"), "bunny.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("sfera");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("ceasca");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.
    cycle = 0;
}


void Tema1::FrameStart()
{
}


void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(color[0], color[1], color[2], color[3]);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    
    RenderMesh(meshes[meshess[cycle]], position, glm::vec3(0.1f, 0.1f, 0.1f));

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.

}


void Tema1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 5.0f;
    if(window->MouseHold(GLFW_MOUSE_BUTTON_2))
        return;
    // Treat continuous update based on input
    if(window->KeyHold(GLFW_KEY_W)) {
        position.z -= speed * deltaTime;
    }
    if(window->KeyHold(GLFW_KEY_S)) {
        position.z += speed * deltaTime;
    }
    if(window->KeyHold(GLFW_KEY_D)) {
        position.x += speed * deltaTime;
    }
    if(window->KeyHold(GLFW_KEY_A)) {
        position.x -= speed * deltaTime;
    }
    if(window->KeyHold(GLFW_KEY_Q)) {
        position.y -= speed * deltaTime;
    }
    if(window->KeyHold(GLFW_KEY_E)) {
        position.y += speed * deltaTime;
    }

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.

}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        color[0] = min(color[0] + 0.01f, 1.0f);
        color[1] = min(color[1] + 0.02f, 1.0f);
        color[2] = min(color[2] + 0.04f, 1.0f);
    }

    
    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.

    if(key == GLFW_KEY_M) {
        cycle = (cycle+1) % 3;
    }
    

}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Tema1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
