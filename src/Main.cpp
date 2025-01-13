#include <windowing/Window.h>
#include <windowing/Input.h>
#include <graphics/OpenGL.h>
#include <misc/UI/UI.h>
#include <misc/camera/CameraControl.h>
#include <utils/Log.h>
#include <objects/SphereMesh.h>

#include <imgui.h>

bool initialize()
{
    WindowParams params;
    return Window::initialize(&params) && OpenGL::initialize(pWindow->pRawWindow) && Input::initialize(pWindow->pRawWindow) && UI::initialize(pWindow->pRawWindow);
}

void updateBegin()
{
    Input::update();
    UI::updateBegin();
}

void updateEnd()
{
    UI::updateEnd();
    Window::update();
}

bool ray_intersects_sphere(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& spherePos, float radius) {
    glm::vec3 oc = rayOrigin - spherePos;
    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * glm::dot(oc, rayDirection);
    float c = glm::dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

AABB getSphereAABB(const glm::vec3& position, float radius) {
    return AABB{
        position - glm::vec3(radius, radius, radius),
        position + glm::vec3(radius, radius, radius)
    };
}

AABB getFloorAABB(const glm::vec3& position, float width, float depth) {
    return AABB{
        position - glm::vec3(width / 2, 0, depth / 2),
        position + glm::vec3(width / 2, 0, depth / 2)
    };
}

bool checkAABBCollision(const AABB& a, const AABB& b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
        (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
        (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

int main()
{
    if (!initialize()) return -1;

    auto base_shader = Shader::load_shader("resources/shaders/default.vert", "resources/shaders/default.frag");
    auto light_shader = Shader::load_shader("resources/shaders/light.vert", "resources/shaders/light.frag");

    float background_color[4]{ 0.07f, 0.13f, 0.17f, 1.0f };

    glm::vec3 floorPos = glm::vec3(0, 0, 0);
    glm::vec3 floorRotate = glm::vec3(0, 0, 0);

    glm::vec3 spherePos = glm::vec3(0, 1.f, 0);

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.f, 5.f, 0.f);

    Camera camera = Camera();
    camera.set_field_of_view(90);

    CameraControl cameraControl = CameraControl(&camera);

    Vertex vertices[] =
    { //               COORDINATES           /            COLORS          /           NORMALS         
        Vertex{glm::vec3(-5.0f, 0.0f,  5.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
        Vertex{glm::vec3(-5.0f, 0.0f, -5.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
        Vertex{glm::vec3(5.0f, 0.0f, -5.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
        Vertex{glm::vec3(5.0f, 0.0f,  5.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)}
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector <unsigned int> ind(indices, indices + sizeof(indices) / sizeof(unsigned int));

    Mesh floor(&verts, &ind);

    float rad = 0.5f;
    SphereMesh sphere(rad, 256, glm::vec3(1.f, 1.f, 1.f));
    SphereMesh light(0.25, 256, glm::vec3(1.f, 1.f, 1.f));

    float gravity = -9.81f;
    float jumpForce = 10.0f;
    glm::vec3 sphereVelocity(0.f);

    bool isDragging = false;
    bool lockd = false;

    float theta = 0.0f;
    float phi = 0.0f;

    float sphereDistance = 3.0f;
    glm::vec3 lastMousePos;
    while (Window::is_opened())
    {
        if (Input::jpressed(KeyCode::KEY_Q)) Window::lock_cursor(lockd = !lockd);
        
        cameraControl.update();

        if (Input::clicked(ButtonCode::MOUSE_BUTTON_1) && Window::is_cursorLocked()) {
            glm::vec3 rayOrigin = camera.get_position();
            glm::vec3 rayDirection = camera.screen_to_world(input.mousePosX, input.mousePosY) - rayOrigin;
            rayDirection = glm::normalize(rayDirection);

            if (ray_intersects_sphere(rayOrigin, rayDirection, spherePos, 3)) {
                isDragging = true;
                lastMousePos = camera.screen_to_world(input.mousePosX, input.mousePosY);
            }
        }
        else { isDragging = false; lastMousePos = glm::vec3(0); }

        updateBegin();

        OpenGL::clear_color(background_color[0], background_color[1], background_color[2], background_color[3]);
        OpenGL::clear();

        /* LIGHT */
        {
            light_shader.bind();

            glm::mat4 light_model = glm::mat4(1.0f);
            {
                light_model = glm::translate(light_model, lightPos);
            }
            light_shader.set_matrix4("model", light_model);
            light_shader.set_vec4("lightColor", lightColor);

            light.draw(&light_shader, &cameraControl, Primitives::TRIANGLES);
            light_shader.unbind();
        }

        /* FLOOR */
        {
            base_shader.bind();
            glm::mat4 floor_model(1.f);
            {
                floor_model = glm::translate(floor_model, floorPos);

                floor_model = glm::rotate(floor_model, glm::radians(floorRotate.x), glm::vec3(1, 0, 0));
                floor_model = glm::rotate(floor_model, glm::radians(floorRotate.y), glm::vec3(0, 1, 0));
                floor_model = glm::rotate(floor_model, glm::radians(floorRotate.z), glm::vec3(0, 0, 1));
            }

            base_shader.set_matrix4("model", floor_model);
            base_shader.set_vec4("lightColor", lightColor);
            base_shader.set_vec3("lightPos", lightPos);

            floor.draw(&base_shader, &cameraControl, Primitives::TRIANGLES);
            base_shader.unbind();
        }

        static auto lastTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        if (isDragging) {
            sphereDistance += input.scroll;
            input.scroll = 0;

            sphereDistance = glm::clamp(sphereDistance, 1.0f, 10.0f);

            glm::vec3 cameraForward = camera.get_front();
            sphereVelocity = (camera.get_position() + cameraForward * sphereDistance) - spherePos;
            sphereVelocity += deltaTime;
        }
        else
        {
            sphereVelocity.y += gravity * deltaTime;

            AABB sphereAABB = getSphereAABB(spherePos, rad);
            AABB floorAABB = getFloorAABB(floorPos, 10.0f, 10.0f);

            if (checkAABBCollision(sphereAABB, floorAABB))
            {
                spherePos.y = floorPos.y + rad;
                sphereVelocity.y = jumpForce;
            }

            if (sphereVelocity.x > 0)
                sphereVelocity.x -= deltaTime;
            else
                sphereVelocity.x = 0;

            if (sphereVelocity.x > 0)
                sphereVelocity.z -= deltaTime;
            else
                sphereVelocity.z = 0;
        }
        
        spherePos += sphereVelocity * deltaTime;

        /* SPHERE */
        {
            base_shader.bind();
            glm::mat4 sphere_model(1.f);
            {
                sphere_model = glm::translate(sphere_model, spherePos);
            }

            base_shader.set_matrix4("model", sphere_model);
            base_shader.set_vec4("lightColor", lightColor);
            base_shader.set_vec3("lightPos", lightPos);

            sphere.draw(&base_shader, &cameraControl, Primitives::TRIANGLES);
            base_shader.unbind();
        }

        ImGui::Begin("settings"); {
            ImGui::InputFloat3("floor position",   glm::value_ptr(floorPos   ));
            ImGui::InputFloat3("floor rotate deg", glm::value_ptr(floorRotate));
            ImGui::InputFloat3("sphere position",  glm::value_ptr(spherePos  ));
            ImGui::ColorEdit4("light color",       glm::value_ptr(lightColor ));
            ImGui::InputFloat3("light position",   glm::value_ptr(lightPos   ));
            ImGui::InputFloat3("sphereVelocity",   glm::value_ptr(sphereVelocity));
            ImGui::InputFloat("gravity", &gravity);
            ImGui::InputFloat("jumpForce", &jumpForce);
        } ImGui::End();

        updateEnd();
    }

    Window::finalize();
}
