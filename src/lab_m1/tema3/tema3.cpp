#include "lab_m1/tema3/tema3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

void Tema3::Init()
{
    {
        const string sourceTextureDir = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "altapa.jpg");
        Texture2D *texture = new Texture2D();
        texture->Load2D(sourceTextureDir.c_str(), GL_REPEAT);
        mapTextures["water"] = texture;
    }

    {
        const string sourceTextureDir = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "lemne.jpg");
        Texture2D *texture = new Texture2D();
        texture->Load2D(sourceTextureDir.c_str(), GL_REPEAT);
        mapTextures["lemne"] = texture;
    }
    {
        const string sourceTextureDir = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "lemnealbie.jpg");
        Texture2D *texture = new Texture2D();
        texture->Load2D(sourceTextureDir.c_str(), GL_REPEAT);
        mapTextures["lemnealbie"] = texture;
    }
    {
        const string sourceTextureDir = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "iarba.jpg");
        Texture2D *texture = new Texture2D();
        texture->Load2D(sourceTextureDir.c_str(), GL_REPEAT);
        mapTextures["iarba"] = texture;
    }
    {
        const string sourceTextureDir = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "luna.jpg");
        Texture2D *texture = new Texture2D();
        texture->Load2D(sourceTextureDir.c_str(), GL_REPEAT);
        mapTextures["luna"] = texture;
    }
    {
        Mesh *mesh = new Mesh("cylinder");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cylinder.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh *mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh *mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh *mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh *mesh = new Mesh("triangle");
        std::vector<glm::vec3> vertices
        {
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(1, 0, 0)
        };
        std::vector<glm::vec3> normals
        {
            glm::vec3(0, 0, 1),
            glm::vec3(0, 0, 1),
            glm::vec3(0, 0, 1)
        };
        std::vector<glm::vec2> textureCoords
        {
            glm::vec2(0, 0),
            glm::vec2(0, 1),
            glm::vec2(1, 0)
        };
        std::vector<unsigned int> indices =
        {
            0, 1, 2
        };
        mesh->InitFromData(vertices, normals, textureCoords, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Shader *shader = new Shader("myshader");
        shader->AddShader(
            PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader.glsl"),
            GL_VERTEX_SHADER);
        shader->AddShader(
            PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader.glsl"),
            GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
    }
    {
        lightPosition[1] = glm::vec3(20.f, 1.f, 12.f);
        lightDirection[1] = glm::vec3(1, -1, 0);
        lightColor[1] = glm::vec3(1, 1, 1);
        type[1] = 0;
        lightPosition[0] = glm::vec3(30.f, 20.f, 0.f);
        lightDirection[0] = glm::vec3(-1, -1, 0);
        lightColor[0] = glm::vec3(1, 1, 1);
        type[0] = 2;
    }
    {
        becLightColor = glm::vec3(0, 1, 0);
    }

}

void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::RenderLighthouse(float baseHeight)
{
    becLightPos = glm::vec3(0, baseHeight, 0);
    becLightDir = glm::vec3(0, 1, 0);

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0, 0, 0);
    modelMatrix *= transform3D::Scale(1.f, baseHeight, 1.f);
    RenderSimpleMesh(meshes["cylinder"], shaders["myshader"], modelMatrix, mapTextures["lemne"]);
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0, baseHeight, 0);
    modelMatrix *= transform3D::Scale(1.f, 0.75f, 1.f);
    RenderSimpleMesh(meshes["cylinder"], shaders["myshader"], modelMatrix, NULL);
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0, baseHeight + 0.75, 0);
    modelMatrix *= transform3D::Scale(1.f, 1.f, 1.f);
    RenderSimpleMesh(meshes["cylinder"], shaders["myshader"], modelMatrix, mapTextures["lemnealbie"]);
}

void Tema3::RenderBoat(int i) {
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(lightPosition[i].x, lightPosition[i].y - 1, lightPosition[i].z);
    modelMatrix *= transform3D::Scale(3.f, .5f, 0.2f);
    RenderSimpleMesh(meshes["box"], shaders["myshader"], modelMatrix, mapTextures["lemne"]);
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(lightPosition[i].x, lightPosition[i].y - 1, lightPosition[i].z);
    modelMatrix *= transform3D::Scale(1.5f, 2.5f, 1.f);
    RenderSimpleMesh(meshes["triangle"], shaders["myshader"], modelMatrix, NULL);
}

void Tema3::Update(float deltaTimeSeconds)
{
    RenderLighthouse(4.f);
    RenderBoat(1);
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.05f, 0.05f, 0.05f);
    modelMatrix *= transform3D::RotateOX(RADIANS(90));
    modelMatrix *= transform3D::Scale(100.f, 100.f, 100.f);
    RenderSimpleMesh(meshes["quad"], shaders["myshader"], modelMatrix, mapTextures["water"]);
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Scale(10.f, 1.f, 10.f);
    RenderSimpleMesh(meshes["sphere"], shaders["myshader"], modelMatrix, mapTextures["iarba"]);
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(lightPosition[0].x, lightPosition[0].y, lightPosition[0].z);
    modelMatrix *= transform3D::Scale(2.f, 2.f, 2.f);
    RenderSimpleMesh(meshes["sphere"], shaders["myshader"], modelMatrix, mapTextures["luna"]);
}

void Tema3::FrameEnd()
{
    // DrawCoordinateSystem();
}

void Tema3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D* texture1)
{
    if (!mesh || !shader || !shader->program)
        return;
    int moving = 0;

    if(!strncmp(mesh->GetMeshID(), "quad", 4))
        moving = 1;
    glUseProgram(shader->program);

    GLint location = glGetUniformLocation(shader->program, "moving");
    glUniform1i(location, moving);

    GLint modelLocation = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    GLint viewLocation = glGetUniformLocation(shader->program, "View");
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


    GLint timeLocation = glGetUniformLocation(shader->program, "Time");
    glUniform1f(timeLocation, Engine::GetElapsedTime());
    int hasTexture = 0;
    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
        hasTexture = 1;
    }
    glUniform1i(glGetUniformLocation(shader->program, "has_texture"), hasTexture);

    int lightPositionLocation = glGetUniformLocation(shader->program, "point_light_pos");
    glUniform3fv(lightPositionLocation, NUMBER_OF_LIGHTSS, glm::value_ptr(lightPosition[0]));

    int lightDirectionLocation = glGetUniformLocation(shader->program, "point_light_dir");
    glUniform3fv(lightDirectionLocation, NUMBER_OF_LIGHTSS, glm::value_ptr(lightDirection[0]));
    
    int lightColorLocation = glGetUniformLocation(shader->program, "point_light_color");
    glUniform3fv(lightColorLocation, NUMBER_OF_LIGHTSS, glm::value_ptr(lightColor[0]));

    int becLightPosLocation = glGetUniformLocation(shader->program, "bec_far_pos");
    glUniform3fv(becLightPosLocation, 1, glm::value_ptr(becLightPos));

    int becLightDirLocation = glGetUniformLocation(shader->program, "bec_far_dir");
    glUniform3fv(becLightDirLocation, 1, glm::value_ptr(becLightDir));

    int becLightColorLocation = glGetUniformLocation(shader->program, "bec_far_color");
    glUniform3fv(becLightColorLocation, 1, glm::value_ptr(becLightColor));

    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);


    int typeLocation = glGetUniformLocation(shader->program, "type");
    glUniform1iv(typeLocation, NUMBER_OF_LIGHTSS, &type[0]);

    int materialShininessLocation = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(materialShininessLocation, materialShininess);
    
    int materialKdLocation = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(materialKdLocation, materialKd);

    int materialKsLocation = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(materialKsLocation, materialKs);

    int cutoffLocation = glGetUniformLocation(shader->program, "cutoff");
    glUniform1f(cutoffLocation, cut_off);

    // mesh->Render();
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 14;

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
        // TODO(student): Set any other keys that you might need
    }
}

void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
}

void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}
