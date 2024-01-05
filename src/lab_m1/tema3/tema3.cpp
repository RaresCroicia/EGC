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

    camera = new implemented::CameraT3();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
    {
        const string sourceTextureDir = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "apa3.jpg");
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
        const string sourceTextureDir = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "panza.jpg");
        Texture2D *texture = new Texture2D();
        texture->Load2D(sourceTextureDir.c_str(), GL_REPEAT);
        mapTextures["panza"] = texture;
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
        moonLightPos = glm::vec3(100.f, 35.f, 0.f);
        moonLightDir = glm::vec3(-1, -0.1f, 0);
        moonLightColor = glm::vec3(1, 1, 1);
    }
    {
        spotFarColor1 = glm::vec3(1, 1, 1);
        spotFarColor2 = glm::vec3(1, 1, 1);
        becLightColor = glm::vec3(1, 1, 1);
    }
    {
        srand(time(NULL));
        for(int i = 0; i < NUMBER_OF_BOATS; i++) {
            boatAngles[i] = rand() % 360;
            bool ok = false;
            while(!ok) {
                ok = true;
                boatDistances[i] = rand() % 360 / 7;
                if(boatDistances[i] < 10) {
                    ok = false;
                    continue;
                }
                for(int j = 0; j < i; j++) {
                    if(abs(boatDistances[i] - boatDistances[j]) < 7) {
                        ok = false;
                        break;
                    }
                }
            }
            boatDirections[i] = rand() % 2;
            if(boatDirections[i] == 0)
                boatDirections[i] = -1;
            lightColor[i] = glm::vec3(rand() % 100 / 100.f, rand() % 100 / 100.f, rand() % 100 / 100.f);
            boatSpeeds[i] = rand() % 100 / 100.f + 0.5f;
        }
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
    spotFarPos1 = glm::vec3(0, baseHeight, 0);
    spotFarDir1 = glm::vec3(0, -0.1, 1);
    spotFarPos2 = glm::vec3(0, baseHeight, 0);
    spotFarDir2 = glm::vec3(0, -0.1, -1);

    angle = angle < 360 ? angle + 1.5 : 0;

    spotFarDir1 = transform3D::RotateOY(RADIANS(angle)) * glm::vec4(spotFarDir1, 1);
    spotFarDir2 = transform3D::RotateOY(RADIANS(angle)) * glm::vec4(spotFarDir2, 1);

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0, 0, 0);
    modelMatrix *= transform3D::Scale(1.f, baseHeight, 1.f);
    RenderSimpleMesh(meshes["cylinder"], shaders["myshader"], modelMatrix, mapTextures["lemne"], 0);
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0, baseHeight, 0);
    modelMatrix *= transform3D::Scale(1.f, 0.75f, 1.f);
    RenderSimpleMesh(meshes["cylinder"], shaders["myshader"], modelMatrix, NULL, 0);
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0, baseHeight + 0.75, 0);
    modelMatrix *= transform3D::Scale(1.f, 1.f, 1.f);
    RenderSimpleMesh(meshes["cylinder"], shaders["myshader"], modelMatrix, mapTextures["lemnealbie"], 0);
}

void Tema3::RenderBoat(int i) {
    glm::mat4 modelMatrix = glm::mat4(1);
    boatAngles[i] += boatDirections[i] * boatSpeeds[i]; 
    if (boatAngles[i] >= 360) boatAngles[i] -= 360;
    if (boatAngles[i] < 0) boatAngles[i] += 360;

    float angleInRadians = RADIANS(boatAngles[i]);
    float x = boatDistances[i] * cos(angleInRadians);
    float z = boatDistances[i] * sin(angleInRadians);
    
    float orientationAngle = angleInRadians + boatDirections[i] * RADIANS(90);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(x, 0, z);
    modelMatrix *= transform3D::RotateOY(-orientationAngle);
    modelMatrix *= transform3D::Scale(3.f, .5f, 1.5f);
    lightPosition[i] = glm::vec3(x, 1, z);
    RenderSimpleMesh(meshes["box"], shaders["myshader"], modelMatrix, mapTextures["lemne"], 0);
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(x, 0, z);
    modelMatrix *= transform3D::RotateOY(-orientationAngle);
    modelMatrix *= transform3D::Scale(1.5f, 2.5f, 1.f);
    RenderSimpleMesh(meshes["triangle"], shaders["myshader"], modelMatrix, mapTextures["panza"], 0);
}

void Tema3::Update(float deltaTimeSeconds)
{
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
    RenderLighthouse(4.f);
    for(int i = 0; i < NUMBER_OF_BOATS; i++) {
        RenderBoat(i);
    }
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.05f, 0.05f, 0.05f);
    modelMatrix *= transform3D::RotateOX(RADIANS(90));
    modelMatrix *= transform3D::Scale(100.f, 100.f, 100.f);
    RenderSimpleMesh(meshes["quad"], shaders["myshader"], modelMatrix, mapTextures["water"], 0);
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Scale(5.f, 1.f, 5.f);
    RenderSimpleMesh(meshes["sphere"], shaders["myshader"], modelMatrix, mapTextures["iarba"], 0);
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(moonLightPos.x, moonLightPos.y, moonLightPos.z);
    modelMatrix *= transform3D::Scale(12.f, 12.f, 12.f);
    RenderSimpleMesh(meshes["sphere"], shaders["myshader"], modelMatrix, mapTextures["luna"], 0);
    projectionMatrix = glm::ortho(-10.f, 10.f, -10.f, 10.f, 0.01f, 200.f);
    glLoadIdentity();
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(1, 0, 0);
    modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0);
    RenderSimpleMesh(meshes["quad"], shaders["myshader"], modelMatrix, mapTextures["water"], 1);
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);

}

void Tema3::FrameEnd()
{
    // DrawCoordinateSystem();
}

void Tema3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D* texture1, int isBoat)
{
    if (!mesh || !shader || !shader->program)
        return;
    int moving = 0;
    int isMoon = 0;
    if(!strncmp(mesh->GetMeshID(), "quad", 4))
        moving = 1;
    if(texture1 == mapTextures["luna"])
        isMoon = 1;

    glUseProgram(shader->program);

    GLint location = glGetUniformLocation(shader->program, "moving");
    glUniform1i(location, moving);

    GLint isMoonLocation = glGetUniformLocation(shader->program, "isMoon");
    glUniform1i(isMoonLocation, isMoon);

    GLint isBoatLocation = glGetUniformLocation(shader->program, "isBoat");
    glUniform1i(isBoatLocation, isBoat);


    GLint modelLocation = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    GLint viewLocation = glGetUniformLocation(shader->program, "View");
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

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
    glUniform3fv(lightPositionLocation, NUMBER_OF_BOATS, glm::value_ptr(lightPosition[0]));

    int lightDirectionLocation = glGetUniformLocation(shader->program, "point_light_dir");
    glUniform3fv(lightDirectionLocation, NUMBER_OF_BOATS, glm::value_ptr(lightDirection[0]));
    
    int lightColorLocation = glGetUniformLocation(shader->program, "point_light_color");
    glUniform3fv(lightColorLocation, NUMBER_OF_BOATS, glm::value_ptr(lightColor[0]));

    int becLightPosLocation = glGetUniformLocation(shader->program, "bec_far_pos");
    glUniform3fv(becLightPosLocation, 1, glm::value_ptr(becLightPos));

    int becLightDirLocation = glGetUniformLocation(shader->program, "bec_far_dir");
    glUniform3fv(becLightDirLocation, 1, glm::value_ptr(becLightDir));

    int becLightColorLocation = glGetUniformLocation(shader->program, "bec_far_color");
    glUniform3fv(becLightColorLocation, 1, glm::value_ptr(becLightColor));

    int spotFarPos1Location = glGetUniformLocation(shader->program, "spot_far_pos1");
    glUniform3fv(spotFarPos1Location, 1, glm::value_ptr(spotFarPos1));

    int spotFarDir1Location = glGetUniformLocation(shader->program, "spot_far_dir1");
    glUniform3fv(spotFarDir1Location, 1, glm::value_ptr(spotFarDir1));

    int spotFarColor1Location = glGetUniformLocation(shader->program, "spot_far_color1");
    glUniform3fv(spotFarColor1Location, 1, glm::value_ptr(spotFarColor1));

    int spotFarPos2Location = glGetUniformLocation(shader->program, "spot_far_pos2");
    glUniform3fv(spotFarPos2Location, 1, glm::value_ptr(spotFarPos2));

    int spotFarDir2Location = glGetUniformLocation(shader->program, "spot_far_dir2");
    glUniform3fv(spotFarDir2Location, 1, glm::value_ptr(spotFarDir2));

    int spotFarColor2Location = glGetUniformLocation(shader->program, "spot_far_color2");
    glUniform3fv(spotFarColor2Location, 1, glm::value_ptr(spotFarColor2));

    int moonLightPosLocation = glGetUniformLocation(shader->program, "moon_light_pos");
    glUniform3fv(moonLightPosLocation, 1, glm::value_ptr(moonLightPos));

    int moonLightDirLocation = glGetUniformLocation(shader->program, "moon_light_dir");
    glUniform3fv(moonLightDirLocation, 1, glm::value_ptr(moonLightDir));

    int moonLightColorLocation = glGetUniformLocation(shader->program, "moon_light_color");
    glUniform3fv(moonLightColorLocation, 1, glm::value_ptr(moonLightColor));

    glm::vec3 eyePosition = camera->GetTargetPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

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

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W))
		{
			// Translate the camera forward
			camera->TranslateForward(deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_A))
		{
			// Translate the camera to the left
			camera->TranslateRight(-deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_S))
		{
			// Translate the camera backwards
			camera->TranslateForward(-deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_D))
		{
			// Translate the camera to the right
			camera->TranslateRight(deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_Q))
		{
			// Translate the camera down
			camera->TranslateUpward(-deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_E))
		{
			// Translate the camera up
			camera->TranslateUpward(deltaTime * cameraSpeed);
		}
    }

    if (window->KeyHold(GLFW_KEY_I)) {
        spotFarColor1.r = spotFarColor1.r < 1 ? spotFarColor1.r + 0.005 : 1;
        spotFarColor2 = spotFarColor1;
        becLightColor = spotFarColor1;
        cout << "(" << spotFarColor1.r << " " << spotFarColor1.g << " " << spotFarColor1.b << ")" << endl;
    }
    if (window->KeyHold(GLFW_KEY_O)) {
        spotFarColor1.g = spotFarColor1.g < 1 ? spotFarColor1.g + 0.005 : 1;
        spotFarColor2 = spotFarColor1;
        becLightColor = spotFarColor1;
        cout << "(" << spotFarColor1.r << " " << spotFarColor1.g << " " << spotFarColor1.b << ")" << endl;
    }
    if (window->KeyHold(GLFW_KEY_P)) {
        spotFarColor1.b = spotFarColor1.b < 1 ? spotFarColor1.b + 0.005 : 1;
        spotFarColor2 = spotFarColor1;
        becLightColor = spotFarColor1;
        cout << "(" << spotFarColor1.r << " " << spotFarColor1.g << " " << spotFarColor1.b << ")" << endl;
    }
    if (window->KeyHold(GLFW_KEY_J)) {
        spotFarColor1.r = spotFarColor1.r > 0 ? spotFarColor1.r - 0.005 : 0;
        spotFarColor2 = spotFarColor1;
        becLightColor = spotFarColor1;
        cout << "(" << spotFarColor1.r << " " << spotFarColor1.g << " " << spotFarColor1.b << ")" << endl;
    }
    if (window->KeyHold(GLFW_KEY_K)) {
        spotFarColor1.g = spotFarColor1.g > 0 ? spotFarColor1.g - 0.005 : 0;
        spotFarColor2 = spotFarColor1;
        becLightColor = spotFarColor1;
        cout << "(" << spotFarColor1.r << " " << spotFarColor1.g << " " << spotFarColor1.b << ")" << endl;
    }
    if (window->KeyHold(GLFW_KEY_L)) {
        spotFarColor1.b = spotFarColor1.b > 0 ? spotFarColor1.b - 0.005 : 0;
        spotFarColor2 = spotFarColor1;
        becLightColor = spotFarColor1;
        cout << "(" << spotFarColor1.r << " " << spotFarColor1.g << " " << spotFarColor1.b << ")" << endl;
    }
    
}

void Tema3::OnKeyPress(int key, int mods)
{
    
}

void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
            camera->RotateFirstPerson_OY(-deltaX * sensivityOY);

        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
        }
    }
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
