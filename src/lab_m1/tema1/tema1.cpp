#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::CreateScene(glm::ivec2 resolution)
{

    {
        float startingPointX = 1000;
        float startingPointY = 300;
        float squareSide = 50;
        Square romb = Square("romb", glm::vec3(startingPointX, startingPointY, 0), squareSide, RED, true);
        Mesh *rombMesh = CreateRomb(romb.name, romb.leftBottomCorner, romb.length, romb.color, romb.fill);
        AddMeshToList(rombMesh); 
    }
    // Main Squares
    {
        float startingPointX = 100;
        float startingPointY = 60;
        float squareSidePercent = 20;
        float distanceBetweenSquares = 10;
        float squareSide = squareSidePercent * fmin(resolution.x, resolution.y) / 100;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                int index = i * 3 + j;
                Square squareObject = Square("squareArena" + std::to_string(index),
                                             glm::vec3(startingPointX + j * (squareSide + distanceBetweenSquares),
                                                       startingPointY + i * (squareSide + distanceBetweenSquares), 0),
                                             squareSide, RED, true);
                squareObject.id = index;
                squaresArena.push_back(squareObject);
                Mesh *square = CreateSquare(squareObject.name, squareObject.leftBottomCorner, squareObject.length, squareObject.color, squareObject.fill);
                AddMeshToList(square);
            }
        }
    }

    {
        float startingPointX = 100;
        float squareSidePercent = 12;
        float distanceBetweenSquares = 60;
        float squareSide = squareSidePercent * fmin(resolution.x, resolution.y) / 100;
        float startingPointY = resolution.y - squareSide - 50;
        for (int i = 0; i < 4; i++)
        {
            Square squareObject = Square("squareWeapon" + std::to_string(i),
                                         glm::vec3(startingPointX + i * (squareSide + 60),
                                                   startingPointY, 0),
                                         squareSide, RED, false);
            squaresShowcase.push_back(squareObject);
            Mesh *square = CreateSquare(squareObject.name, squareObject.leftBottomCorner, squareObject.length, squareObject.color, squareObject.fill);
            AddMeshToList(square);
        }
    }

    {
        float startingPointX = 20;
        float startingPointY = 60;
        float squareSidePercent = 20;
        float distanceBetweenSquares = 10;
        float squareSide = squareSidePercent * fmin(resolution.x, resolution.y) / 100;
        float width = 60;
        float height = squareSide * 3 + distanceBetweenSquares * 2;

        rectangle = Rectangle("rectangle", glm::vec3(startingPointX, startingPointY, 0), width, height, RED, true);

        Mesh *rectangleMesh = CreateRectangle(rectangle.name, rectangle.leftBottomCorner, rectangle.width, rectangle.height, rectangle.color, rectangle.fill);
        AddMeshToList(rectangleMesh);
    }

    {
        vector<glm::vec3> colors = {ORANGE, BLUE, YELLOW, MAGENTA};
        for (int i = 0; i < 4; i++)
        {
            float startingPointX = squaresShowcase[i].leftBottomCorner.x + squaresShowcase[i].length / 8;
            float startingPointY = squaresShowcase[i].leftBottomCorner.y + squaresShowcase[i].length / 2;
            squaresShowcase[i].colorInside = colors[i];
            float length = squaresShowcase[i].length / 2 - 2;
            rombLength = length;
            Square squareObject = Square("weaponShowcase" + std::to_string(i),
                                         glm::vec3(startingPointX, startingPointY, 1),
                                         length, colors[i], true);
            showcaseWeapons.push_back(squareObject);
            Mesh *square = CreateRomb(squareObject.name, squareObject.leftBottomCorner, squareObject.length, squareObject.color, squareObject.fill);
            AddMeshToList(square);
        }
    }

    {
        float distanceBetweenLives = 10;
        for (int i = 0; i < 3; i++)
        {
            float startingPointX = resolution.x - (3 - i) * (squaresShowcase[0].leftBottomCorner.x + distanceBetweenLives);
            float startingPointY = squaresShowcase[0].leftBottomCorner.y;
            float length = squaresShowcase[0].length;
            Square squareObject = Square("life" + std::to_string(i),
                                         glm::vec3(startingPointX, startingPointY, 1),
                                         length, RED, true);
            lives.push_back(squareObject);
            Mesh *square = CreateSquare(squareObject.name, squareObject.leftBottomCorner, squareObject.length, squareObject.color, squareObject.fill);
            AddMeshToList(square);
        }
    }
}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    CreateScene(resolution);
    timeSinceLastSpawn = 0.0f;
    spawnInterval = 1.5f;
    pointsEverCreated = 0;
    isPressed = false;
    draggedWeapon = Square("draggedWeapon", glm::vec3(0, 0, 1), rombLength, RED, true);
    Mesh *draggedWeaponMesh = CreateRomb(draggedWeapon.name, draggedWeapon.leftBottomCorner, draggedWeapon.length, draggedWeapon.color, draggedWeapon.fill);
    AddMeshToList(draggedWeaponMesh);
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

template <typename T>
void Tema1::Render(vector<T> array, char status)
{
    glm::mat3 modelMatrix = glm::mat3(1);
    bool isRotation = (status & (1 << 2)) == (1 << 2);
    bool isScale = (status & (1 << 1)) == (1 << 1);
    bool isTranslation = (status & 1) == (1 << 0);

    for (T element : array)
    {

        if (isRotation && isTranslation)
        {
            modelMatrix = Translate(element.center.x, element.center.y) * Rotate(element.angle) * Translate(-element.center.x, -element.center.y) * glm::mat3(1);
        }
        RenderMesh2D(meshes[element.name], shaders["VertexColor"], modelMatrix);
    }
}

template <typename T>
void Tema1::Render(T element, char status)
{
    glm::mat3 modelMatrix = glm::mat3(1);
    bool isRotation = (status & ROTATION) == ROTATION;
    bool isScale = (status & SCALE) == SCALE;
    bool isTranslation = (status & TRANSLATION) == TRANSLATION;
    if (isTranslation)
    {
        modelMatrix = Translate(pressedMouseX, pressedMouseY) * glm::mat3(1);
    }
    RenderMesh2D(meshes[element.name], shaders["VertexColor"], modelMatrix);
}

void Tema1::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    {
        Render(squaresArena, 0);
        Render(squaresShowcase, 0);
        Render(rectangle, 0);
        Render(showcaseWeapons, 0);
        Render(lives, 0);
        Render(points, ROTATION | TRANSLATION);
        Render(usablePoints, 0);
        if(isPressed) {
            Render(draggedWeapon, 0);
        }
        Render(weapons, 0);
    }
    {
        timeSinceLastSpawn += deltaTimeSeconds;
        if (timeSinceLastSpawn >= spawnInterval && points.size() < MAX_STARS_AVAILABLE)
        {
            SpawnPoint();
            timeSinceLastSpawn = 0.0f;
        }
    }
}

bool Tema1::AddPoint()
{
    if (usablePoints.size() == 0)
    {
        float X = window->GetResolution().x - 3 * (squaresShowcase[0].leftBottomCorner.x + 30);
        float Y = squaresShowcase[0].leftBottomCorner.y - squaresShowcase[0].length / 2;
        Circle point = Circle("usablepoint" + std::to_string(usablePoints.size()), glm::vec3(X, Y, 0), 20, YELLOW, true);
        point.angle = 180;
        usablePoints.push_back(point);
        Mesh *pointMesh = CreateStar(point.name, point.center, point.radius, point.color, point.fill);
        meshes[point.name] = pointMesh;
        return true;
    }
    else if (usablePoints.size() < MAX_POINTS)
    {
        float X = usablePoints[usablePoints.size() - 1].center.x + 2 * usablePoints[usablePoints.size() - 1].radius;
        float Y = usablePoints[usablePoints.size() - 1].center.y;
        Circle point = Circle("usablepoint" + std::to_string(usablePoints.size()), glm::vec3(X, Y, 0), 20, YELLOW, true);
        point.angle = 180;
        usablePoints.push_back(point);
        Mesh *pointMesh = CreateStar(point.name, point.center, point.radius, point.color, point.fill);
        meshes[point.name] = pointMesh;
        return true;
    }
    return false;
}

bool Tema1::RemovePoint()
{
    if (usablePoints.size() > 0)
    {
        meshes.erase(usablePoints[usablePoints.size() - 1].name);
        usablePoints.erase(usablePoints.begin() + usablePoints.size() - 1);
        return true;
    }
    return false;
}

void Tema1::SpawnPoint()
{
    srand(time(NULL));
    int chance = 50;
    int random = rand() % 100;
    if (random <= chance)
    {
        if (points.size() <= MAX_STARS_AVAILABLE)
        {
            float x = max(rand() % window->GetResolution().x, window->GetResolution().x / 2);
            float y = rand() % window->GetResolution().y;
            float radius = 50;
            Circle point = Circle("point" + std::to_string(pointsEverCreated++), glm::vec3(x, y, 0), radius, YELLOW, true);
            point.angle = 180;
            points.push_back(point);
            Mesh *pointMesh = CreateStar(point.name, point.center, point.radius, point.color, point.fill);
            meshes[point.name] = pointMesh;
        }
    }
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
}

void Tema1::OnKeyPress(int key, int mods)
{
}

void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    HandleStillPressed(mouseX, mouseY);
}

void Tema1::HandleClickArena(int mouseX, int mouseY)
{
    for (int i = 0; i < squaresArena.size(); i++)
    {
        Square square = squaresArena[i];
        if (mouseX >= square.leftBottomCorner.x && mouseX <= square.leftBottomCorner.x + square.length &&
            mouseY >= square.leftBottomCorner.y && mouseY <= square.leftBottomCorner.y + square.length)
        {
            if (RemovePoint())
            {
                if (square.color == RED)
                    square.color = GREEN;
                else
                    square.color = RED;
                Mesh *squareMesh = CreateSquare(square.name, square.leftBottomCorner, square.length, square.color, square.fill);
                meshes[square.name] = squareMesh;
                squaresArena[i] = square;
            }
            break;
        }
    }
}

void Tema1::HandleClickPoints(int mouseX, int mouseY)
{
    for (int i = 0; i < points.size(); i++)
    {
        Circle point = points[i];
        if (mouseX >= point.center.x - point.radius && mouseX <= point.center.x + point.radius &&
            mouseY >= point.center.y - point.radius && mouseY <= point.center.y + point.radius)
        {
            if (AddPoint())
            {
                meshes.erase(point.name);
                points.erase(points.begin() + i);
            }
            break;
        }
    }
}

void Tema1::HandleClickShowcase(int mouseX, int mouseY)
{
    for (Square square : squaresShowcase)
    {
        if (mouseX >= square.leftBottomCorner.x && mouseX <= square.leftBottomCorner.x + square.length &&
            mouseY >= square.leftBottomCorner.y && mouseY <= square.leftBottomCorner.y + square.length)
        {
            {
                isPressed = true;
                draggedWeapon.leftBottomCorner = glm::vec3(mouseX, mouseY, 1);
                draggedWeapon.color = square.colorInside;
                meshes[draggedWeapon.name] = CreateRomb(draggedWeapon.name, draggedWeapon.leftBottomCorner, draggedWeapon.length, draggedWeapon.color, draggedWeapon.fill);
            }

            break;
        }
    }
}

void Tema1::HandleStillPressed(int mouseX, int mouseY) {
    mouseY = window->GetResolution().y - mouseY;
    if(isPressed) {
        draggedWeapon.leftBottomCorner = glm::vec3(mouseX, mouseY, 1);
        Mesh *draggedWeaponMesh = CreateRomb(draggedWeapon.name, draggedWeapon.leftBottomCorner, draggedWeapon.length, draggedWeapon.color, draggedWeapon.fill);
        meshes[draggedWeapon.name] = draggedWeaponMesh;
    }
}

void Tema1::HandleClickRemoveArena(int mouseX, int mouseY) {
    mouseY = window->GetResolution().y - mouseY;
    for (Square square : squaresArena)
        if(mouseX >= square.leftBottomCorner.x && mouseX <= square.leftBottomCorner.x + square.length &&
            mouseY >= square.leftBottomCorner.y && mouseY <= square.leftBottomCorner.y + square.length) {
                if(square.colorInside != BLACK) {
                    square.colorInside = BLACK;
                    weapons.erase(weapons.begin() + square.id);
                }
                break;
            }
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    auto res = window->GetResolution();
    mouseY = res.y - mouseY;
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT))
    {
        HandleClickArena(mouseX, mouseY);
        HandleClickPoints(mouseX, mouseY);
        HandleClickShowcase(mouseX, mouseY);
    }
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT))
    {
        HandleClickRemoveArena(mouseX, mouseY);
    }
}

// 131 491

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    mouseY = window->GetResolution().y - mouseY;
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT))
    {
        if (isPressed) {
            isPressed = false;
            for (Square square : squaresArena)
            {
                if (mouseX >= square.leftBottomCorner.x && mouseX <= square.leftBottomCorner.x + square.length &&
                    mouseY >= square.leftBottomCorner.y && mouseY <= square.leftBottomCorner.y + square.length)
                {
                    if (square.colorInside == BLACK)
                    {
                        // create weapon and put it there
                        square.colorInside = draggedWeapon.color;
                        glm::vec3 position = square.leftBottomCorner + glm::vec3(square.length/4, square.length/2, 3);
                        Square weapon = Square("weapon" + std::to_string(square.id), position, rombLength + 5, square.colorInside, true);
                        weapons.push_back(weapon);
                        Mesh *weaponMesh = CreateRomb(weapon.name, weapon.leftBottomCorner, weapon.length, weapon.color, weapon.fill);
                        meshes[weapon.name] = weaponMesh;
                    }
                    break;
                }
            }
        }
    }
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}

Mesh *Tema1::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
        {
            VertexFormat(corner, color),
            VertexFormat(corner + glm::vec3(length, 0, 0), color),
            VertexFormat(corner + glm::vec3(length, length, 0), color),
            VertexFormat(corner + glm::vec3(0, length, 0), color)};

    Mesh *square = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3};

    if (!fill)
    {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else
    {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh *Tema1::CreateRomb(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
        {
            VertexFormat(corner, color),
            VertexFormat(corner + glm::vec3(length / 2, length, 0), color),
            VertexFormat(corner + glm::vec3(length, 0, 0), color),
            VertexFormat(corner + glm::vec3(length / 2, -length, 0), color),
            VertexFormat(corner + glm::vec3(length / 2, -length / 4, 0), color),
            VertexFormat(corner + glm::vec3(length / 2, length / 4, 0), color),
            VertexFormat(corner + glm::vec3(3 * length / 2, length / 4, 0), color),
            VertexFormat(corner + glm::vec3(3 * length / 2, -length / 4, 0), color)};

    Mesh *square = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3};

    if (!fill)
    {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else
    {
        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(4);
        indices.push_back(6);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh *Tema1::CreateRectangle(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float width,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
        {
            VertexFormat(corner, color),
            VertexFormat(corner + glm::vec3(width, 0, 0), color),
            VertexFormat(corner + glm::vec3(width, height, 0), color),
            VertexFormat(corner + glm::vec3(0, height, 0), color)};

    Mesh *square = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3};

    if (!fill)
    {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else
    {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh *Tema1::CreateStar(
    const std::string &name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = center;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    int numberOfVertices = 5;
    float angle = 2 * M_PI / numberOfVertices;
    float currentAngle = 0.00f;
    vertices.push_back(VertexFormat(corner, color));
    for (int i = 0; i < numberOfVertices; i++)
    {
        vertices.push_back(VertexFormat(corner + glm::vec3(radius * cos(currentAngle), radius * sin(currentAngle), 0), color));
        currentAngle += angle;
    }

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(4);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(4);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(5);
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(5);

    Mesh *circle = new Mesh(name);

    if (!fill)
    {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    else
    {
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh *Tema1::CreateEnemy(
    const std::string &name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    glm::vec3 color2,
    bool fill)
{
    glm::vec3 corner = center;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    int numberOfVertices = 6;
    float angle = 2 * M_PI / numberOfVertices;
    float currentAngle = 0.00f;
    vertices.push_back(VertexFormat(corner, color2));
    for (int i = 0; i < numberOfVertices; i++)
    {
        vertices.push_back(VertexFormat(corner + glm::vec3(radius * cos(currentAngle), radius * sin(currentAngle), 0), color));
        currentAngle += angle;
    }

    for (int i = 0; i < numberOfVertices - 1; i++)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }
    indices.push_back(0);
    indices.push_back(numberOfVertices);
    indices.push_back(1);

    currentAngle = 0.00f;
    corner += glm::vec3(0, 0, 1);
    vertices.push_back(VertexFormat(corner, color));
    for (int i = 0; i < numberOfVertices; i++)
    {
        vertices.push_back(VertexFormat(corner + glm::vec3(radius * 0.75 * cos(currentAngle), radius * 0.75 * sin(currentAngle), 1), color2));
        currentAngle += angle;
    }
    for (int i = 7; i < 12; i++)
    {
        indices.push_back(7);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }
    indices.push_back(7);
    indices.push_back(13);
    indices.push_back(8);

    Mesh *circle = new Mesh(name);

    if (!fill)
    {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    else
    {
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}
