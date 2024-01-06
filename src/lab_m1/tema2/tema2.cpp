#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
    fov = 60.f;
    zFar = 200.f;
    zNear = .01f;
    translateX = 0;
    translateY = 0;
    translateZ = 0;
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;
    distanceX = 0;
    distanceY = 3.5f;
    distanceZ = 5.f;

    camera = new implemented::CameraT2();
    camera->distanceToTarget = sqrt(distanceY * distanceY + distanceZ * distanceZ);
    {
        Mesh *mesh = new Mesh("tankcap");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "personals"), "cap.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh *mesh = new Mesh("tankcorp");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "personals"), "corp.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh *mesh = new Mesh("tankroti");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "personals"), "roti.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh *mesh = new Mesh("tankteava");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "personals"), "teava.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh *mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Shader *shader = new Shader("myshader");
        shader->AddShader(
            PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"),
            GL_VERTEX_SHADER);
        shader->AddShader(
            PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"),
            GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    camera->Set(glm::vec3(translateX + distanceX, translateY + distanceY, translateZ + distanceZ), glm::vec3(0, 1, 1),
                glm::vec3(0, 1, 1));
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    modelMatrix *= transform3D::RotateOY(RADIANS(angularStepOY + 90));
    RenderSimpleMesh(meshes["tankcap"], shaders["myshader"], modelMatrix);
    RenderSimpleMesh(meshes["tankcorp"], shaders["myshader"], modelMatrix);
    RenderSimpleMesh(meshes["tankroti"], shaders["myshader"], modelMatrix);
    RenderSimpleMesh(meshes["tankteava"], shaders["myshader"], modelMatrix);
    RenderBullets(deltaTimeSeconds);
}

void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderBullets(float deltaTimeSeconds) {
    float speed = 2.0f;
    for (int i = 0; i < bullets.size(); i++) {
        cout << bullets[i].position.x << " " << bullets[i].position.y << " " << bullets[i].position.z << endl;
        bullets[i].position.x += deltaTimeSeconds * speed * cos(RADIANS(bullets[i].angle));
        bullets[i].position.z += deltaTimeSeconds * speed * sin(RADIANS(bullets[i].angle));
        bullets[i].frames ++;
        if (bullets[i].frames > 1000) {
            bullets.erase(bullets.begin() + i);
            i--;
            continue;
        }
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(bullets[i].position.x, bullets[i].position.y, bullets[i].position.z);
        modelMatrix *= transform3D::Scale(0.05f, 0.05f, 0.05f);
        RenderSimpleMesh(meshes["sphere"], shaders["myshader"], modelMatrix);
    }
}

void Tema2::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    glUseProgram(shader->program);

    GLint modelLocation = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    GLint viewLocation = glGetUniformLocation(shader->program, "View");
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    GLint projectionLocation = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint timeLocation = glGetUniformLocation(shader->program, "Time");
    glUniform1f(timeLocation, Engine::GetElapsedTime());

    glm::vec3 color;
    if (!strncmp(mesh->GetMeshID(), "tankcap", 7))
        color = glm::vec3(0.f, 1.f, 0.f);
    else if (!strncmp(mesh->GetMeshID(), "tankcorp", 8))
        color = glm::vec3(0.f, 0.3f, 0.f);
    else if (!strncmp(mesh->GetMeshID(), "tankroti", 8))
        color = glm::vec3(1.f, 1.f, 1.f);
    else if (!strncmp(mesh->GetMeshID(), "tankteava", 9))
        color = glm::vec3(1.f, 1.f, 1.0f);
    else if (!strncmp(mesh->GetMeshID(), "sphere", 6))
        color = glm::vec3(0.0f, 0.5f, 1.0f);
    else
        color = glm::vec3(0.69f, 0.69f, 0.69f);
    
    int hasColorLocation = glGetUniformLocation(shader->program, "hasColor");
    glUniform1i(hasColorLocation, color != glm::vec3(0.69f, 0.69f, 0.69f) ? 1 : 0);

    int colorLocation = glGetUniformLocation(shader->program, "Color");
    glUniform3fv(colorLocation, 1, glm::value_ptr(color));

    mesh->Render();
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema2::OnInputUpdate(float deltaTime, int mods)
{

    if (window->KeyHold(GLFW_KEY_W))
    {
        translateX -= sin(RADIANS(angularStepOY)) * deltaTime * speed;
        translateZ -= cos(RADIANS(angularStepOY)) * deltaTime * speed;
        camera->MoveForward(deltaTime * speed);
    }
    if (window->KeyHold(GLFW_KEY_S))
    {
        translateX += sin(RADIANS(angularStepOY)) * deltaTime * speed;
        translateZ += cos(RADIANS(angularStepOY)) * deltaTime * speed;
        camera->MoveForward(-deltaTime * speed);
    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        angularStepOY -= deltaTime * rotationSpeed;
        camera->RotateThirdPerson_OY(RADIANS(-deltaTime * rotationSpeed));
    }
    if (window->KeyHold(GLFW_KEY_A))
    {
        angularStepOY += deltaTime * rotationSpeed;
        camera->RotateThirdPerson_OY(RADIANS(deltaTime * rotationSpeed));
    }
}

void Tema2::OnKeyPress(int key, int mods)
{
    if(key == GLFW_KEY_SPACE) {
        cout << "Apas in mortii tai\n";
        Bullet bullet;
        bullet.position = glm::vec3(translateX + distanceX, translateY + distanceY, translateZ + distanceZ);
        bullet.angle = - angularStepOY - 90;
        bullets.push_back(bullet);
    }
}

void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0)
        {
            camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
            camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL)
        {
            camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
        }
    }
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
