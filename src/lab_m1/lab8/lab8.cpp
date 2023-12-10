#include "lab_m1/lab8/lab8.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab8::Lab8()
{
}


Lab8::~Lab8()
{
}

int getRandom() {
    int chance = 33;
    int r = rand() % 100;
    if(r < chance) return -1;
    if(r < 2 * chance) return 0;
    return 1;
}


void Lab8::Init()
{
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab8", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab8", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {
        lightPosition = glm::vec3(0, 1, 1);
        lightDirection = glm::vec3(0, -1, 0);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
    }

    {
        for(int i = 0; i < NUMBER_OF_LIGHTS; i++) {
            point_light_pos[i] = glm::vec3(rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f);
            point_light_color[i] = glm::vec3(rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f);
            point_light_dir[i] = glm::vec3(getRandom(), getRandom(), getRandom());
            type[i] = i == 0 ? 1 : 0;
        }
    }
}


void Lab8::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab8::Update(float deltaTimeSeconds)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        // TODO(student): Add or change the object colors
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix);

    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
        // TODO(student): Add or change the object colors
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix);

    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.25, 0.75, 0.75));
    }

    // Render ground
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
        // TODO(student): Add or change the object colors
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix);

    }

    // Render the point light in the scene
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, point_light_pos[0]);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
    }

    // Render the point light in the scene
    {
        for(int i = 0; i < NUMBER_OF_LIGHTS; i++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, point_light_pos[i]);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
            RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
        }
    }
}


void Lab8::FrameEnd()
{
    DrawCoordinateSystem();
}


void Lab8::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light properties
    int light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    // TODO(student): Set any other shader uniforms that you need
    GLuint point_light_location = glGetUniformLocation(shader->program, "point_light_pos");
    glUniform3fv(point_light_location, NUMBER_OF_LIGHTS, glm::value_ptr(point_light_pos[0]));

    GLuint point_light_color_location = glGetUniformLocation(shader->program, "point_light_color");
    glUniform3fv(point_light_color_location, NUMBER_OF_LIGHTS, glm::value_ptr(point_light_color[0]));

    GLuint point_light_dir_location = glGetUniformLocation(shader->program, "point_light_dir");
    glUniform3fv(point_light_dir_location, NUMBER_OF_LIGHTS, glm::value_ptr(point_light_dir[0]));

    GLuint type_location = glGetUniformLocation(shader->program, "type");
    glUniform1iv(type_location, NUMBER_OF_LIGHTS, &type[0]);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    GLuint cut_off_location = glGetUniformLocation(shader->program, "cut_off");
    glUniform1f(cut_off_location, cut_off);

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab8::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 2;

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
        for(int i = 0; i < NUMBER_OF_LIGHTS; i++) {
        // Control light position using on W, A, S, D, E, Q
            if (window->KeyHold(GLFW_KEY_W)) point_light_pos[i] -= forward * deltaTime * speed;
            if (window->KeyHold(GLFW_KEY_A)) point_light_pos[i] -= right * deltaTime * speed;
            if (window->KeyHold(GLFW_KEY_S)) point_light_pos[i] += forward * deltaTime * speed;
            if (window->KeyHold(GLFW_KEY_D)) point_light_pos[i] += right * deltaTime * speed;
            if (window->KeyHold(GLFW_KEY_E)) point_light_pos[i] += up * deltaTime * speed;
            if (window->KeyHold(GLFW_KEY_Q)) point_light_pos[i] -= up * deltaTime * speed;
            if(window->KeyHold(GLFW_KEY_I))
                point_light_dir[i].x = min(1.0f, point_light_dir[i].x + 0.8f * deltaTime);
            if(window->KeyHold(GLFW_KEY_K))
                point_light_dir[i].x = max(-1.0f, point_light_dir[i].x - 0.8f * deltaTime);
            if(window->KeyHold(GLFW_KEY_J))
                point_light_dir[i].y = min(1.0f, point_light_dir[i].y + 0.8f * deltaTime);
            if(window->KeyHold(GLFW_KEY_L))
                point_light_dir[i].y = max(-1.0f, point_light_dir[i].y - 0.8f * deltaTime);
            if(window->KeyHold(GLFW_KEY_U))
                point_light_dir[i].z = min(1.0f, point_light_dir[i].z + 0.8f * deltaTime);
            if(window->KeyHold(GLFW_KEY_O))
                point_light_dir[i].z = max(-1.0f, point_light_dir[i].z - 0.8f * deltaTime);
        }
        if(window->KeyHold(GLFW_KEY_Z))
            cut_off = max(0.0f, cut_off - 100 * deltaTime);
        if(window->KeyHold(GLFW_KEY_X))
            cut_off = min(90.0f, cut_off + 100 * deltaTime);
        // TODO(student): Set any other keys that you might need

    }
}


void Lab8::OnKeyPress(int key, int mods)
{
    // Add key press event

    // TODO(student): Set keys that you might need
    if(key == GLFW_KEY_F) {
        for(int i = 0; i < NUMBER_OF_LIGHTS; i++)
            type[i] = (type[i] + 1) % 2;
    }


    if(key ==GLFW_KEY_1)
        for(int i = 0; i < NUMBER_OF_LIGHTS; i++)
            point_light_color[i] = glm::vec3(rand()%100/100.f, rand()%100/100.f, rand()%100/100.f);
}


void Lab8::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab8::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab8::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab8::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab8::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab8::OnWindowResize(int width, int height)
{
}
