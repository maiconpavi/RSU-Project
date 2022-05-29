#include "camera.hpp"

#include <iostream>
#include <sstream>
#include <vector>

#include <quaternion/Matrices.h>
#include <solution/solution.hpp>
#include <window/window.hpp>
extern CustomWindow *main_window;

Camera::Camera(glm::vec3 position, glm::vec3 pyr, float fov)
    : m_Position(std::move(position)), m_Fov(fov)
{
    m_Pitch = pyr.x;
    m_Yaw = pyr.y;
    m_Roll = pyr.z;

    UpdateVectors();
}

void Camera::SetLastMousePos(glm::vec2 last)
{
    m_LastX = last.x;
    m_LastY = last.y;
}

void Camera::UpdateVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

    m_Target = glm::normalize(front);
    glm::mat4 roll_mat = glm::rotate(glm::mat4(1.0f), glm::radians(m_Roll), front);
    glm::vec3 w_up = glm::vec3(0.0f, 1.0f, 0.0f);

    m_Right = glm::normalize(glm::cross(m_Target, w_up));
    m_Up = glm::normalize(glm::cross(m_Right, m_Target));
    m_Up = glm::mat3(roll_mat) * m_Up;

    m_Right = glm::normalize(glm::cross(m_Target, m_Up));
}

void Camera::SetPosition(glm::vec3 pos)
{
    m_Position = pos;
}

void Camera::SetTarget(glm::vec3 tar)
{
    m_Target = tar;
}

void Camera::SetPYR(glm::vec3 pyr)
{
    m_Pitch = pyr.x;
    m_Yaw = pyr.y;
    m_Roll = pyr.z;
    UpdateVectors();
}

void Perspective::SetLeftCamera()
{
    LoadMaxMin();
    SetPosition(glm::vec3(0.0f, 0.0f, -abs(lorenz_max_z - lorenz_min_z) / 5));
    SetPYR(glm::vec3(0.0f, 90.0f, 0.0f));
}

void Perspective::SetFrontCamera()
{
    LoadMaxMin();
    SetPosition(glm::vec3(-abs(lorenz_min_x - lorenz_max_x), 0.0f, lorenz_max_z / 2));
    SetPYR(glm::vec3(0.0f, 0.0f, 0.0f));
}

void Perspective::SetTopCamera()
{
    LoadMaxMin();
    SetPosition(glm::vec3(0.0f, 0.8 * abs(lorenz_min_y - lorenz_max_y), lorenz_max_z / 2));
    SetPYR(glm::vec3(-89.999f, 90.0f, 0.0f));
}

Perspective::Perspective(glm::vec3 position, glm::vec3 pyr, float fov)
    : Camera(position, pyr, fov)
{
    m_Type = 0;
    UpdateScroll(0.0f);
};

Ortho::Ortho(glm::vec3 position, glm::vec3 pyr, float fov)
    : Camera(position, pyr, fov)
{
    m_Type = 1;
    UpdateScroll(0.0f);
    m_Quat.set(1, 0, 0, 0);
    r_Speed = 200.0f;
};

Trackball::Trackball(glm::vec3 position, glm::vec3 pyr, float fov)
    : Camera(position, pyr, fov)
{
    m_Type = 2;
    UpdateScroll(0.0f);
    m_Quat.set(1, 0, 0, 0);
    r_Speed = 200.0f;
};

void Perspective::UpdateKeyboard(GLFWwindow *window, float dt)
{
    // Move Foward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_Position += m_Target * m_Speed * dt * (float)speed_up;
    // Move Backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_Position -= m_Target * m_Speed * dt * (float)speed_up;
    // Move Right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_Position += m_Right * m_Speed * dt * (float)speed_up;
    // Move Left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_Position -= m_Right * m_Speed * dt * (float)speed_up;
    // Move Up
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_Position += m_Up * m_Speed * dt * (float)speed_up;
    // Move Down
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_Position -= m_Up * m_Speed * dt * (float)speed_up;

    // Camera Roll
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        m_Roll -= r_Speed * dt;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        m_Roll += r_Speed * dt;

    // Control esquerdo quando acionado duplica a velocidade de movimento, pressionar novamente desativa.
    // if (main_window->tl_key_toggle(GLFW_KEY_LEFT_CONTROL)) {
    //     if ((main_window->tl_key_is_on(GLFW_KEY_LEFT_CONTROL))) {
    //         speed_up = boost;

    //     } else {
    //         speed_up = 1.0f;
    //     }
    // }

    if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) && (pressed == true)) {
        pressed = false;
    } else if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) && (speed_up == 1.0f) &&
               (pressed == false)) {
        speed_up = boost;
        pressed = true;
    } else if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) && (speed_up == boost) &&
               (pressed == false)) {
        speed_up = 1.0f;
        pressed = true;
    }
}

void Ortho::UpdateKeyboard(GLFWwindow *window, float dt)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        Quaternion qx = Quaternion(Vector3(1.0f, 0.0f, 0.0f), -r_Speed * dt * 0.002f);
        m_Quat = qx * m_Quat;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        Quaternion qx = Quaternion(Vector3(1.0f, 0.0f, 0.0f), r_Speed * dt * 0.002f);
        m_Quat = qx * m_Quat;
    }

    // Control esquerdo quando acionado duplica a velocidade de movimento, pressionar novamente desativa.
    if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) && (pressed == true)) {
        pressed = false;
    } else if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) && (speed_up == 1.0f) &&
               (pressed == false)) {
        speed_up = boost;
        pressed = true;
    } else if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) && (speed_up == boost) &&
               (pressed == false)) {
        speed_up = 1.0f;
        pressed = true;
    }
}

void Trackball::UpdateKeyboard(GLFWwindow *window, float dt)
{

    // Camera Roll
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        Quaternion qx = Quaternion(Vector3(1.0f, 0.0f, 0.0f), -r_Speed * dt * 0.002f);
        m_Quat = qx * m_Quat;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        Quaternion qx = Quaternion(Vector3(1.0f, 0.0f, 0.0f), r_Speed * dt * 0.002f);
        m_Quat = qx * m_Quat;
    }

    // Control esquerdo quando acionado duplica a velocidade de movimento, pressionar novamente desativa.
    if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) && (pressed == true)) {
        pressed = false;
    } else if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) && (speed_up == 1.0f) &&
               (pressed == false)) {
        speed_up = boost;
        pressed = true;
    } else if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) && (speed_up == boost) &&
               (pressed == false)) {
        speed_up = 1.0f;
        pressed = true;
    }
}

void Perspective::UpdateMouse(GLFWwindow *window)
{
    double xPos;
    double yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    if (m_FirstMouse) {
        m_LastX = (float)xPos;
        m_LastY = (float)yPos;
        m_FirstMouse = false;
    }

    float xoffset = (float)xPos - m_LastX;
    float yoffset = m_LastY - (float)yPos; // Invertido pois o Y é ao contrário no GLFW

    m_LastX = (float)xPos;
    m_LastY = (float)yPos;

    xoffset *= m_Sensitivity;
    yoffset *= m_Sensitivity;

    m_Yaw += xoffset * cos(glm::radians(m_Roll)) + yoffset * sin(glm::radians(m_Roll));
    m_Pitch += -xoffset * sin(glm::radians(m_Roll)) + yoffset * cos(glm::radians(m_Roll));
    m_Pitch = std::max(std::min(89.99999f, m_Pitch), -89.99999f);

    UpdateVectors();
}

void Ortho::UpdateMouse(GLFWwindow *window)
{
    double xPos;
    double yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    if (MousePressed[GLFW_MOUSE_BUTTON_1] == false && MousePressed[GLFW_MOUSE_BUTTON_2] == false) {
        m_LastX = (float)xPos;
        m_LastY = (float)yPos;
    }

    if (m_FirstMouse) {
        m_LastX = (float)xPos;
        m_LastY = (float)yPos;
        m_FirstMouse = false;
    }

    float xoffset = (float)xPos - m_LastX;
    float yoffset = m_LastY - (float)yPos; // Invertido pois o Y é ao contrário no GLFW

    if (MousePressed[GLFW_MOUSE_BUTTON_1] == true) {
        LoadMaxMin();
        float maximo = std::max(abs(lorenz_max_z - lorenz_min_z), abs(lorenz_min_y - lorenz_max_y));
        // Pitch
        Quaternion qz = Quaternion(Vector3(0.0f, 0.0f, 1.0f),
                                   -yoffset * 0.002f); // rotate along Z

        // Roll
        Quaternion qx = Quaternion(Vector3(1, 0, 0), -yoffset * 0.002f); // rotate along X

        // Yaw
        Quaternion qy = Quaternion(Vector3(0, 1, 0), xoffset * 0.002f); // rotate along y
        m_Quat = qy * qz * m_Quat;
    } else if (MousePressed[GLFW_MOUSE_BUTTON_2] == true) {
        LoadMaxMin();
        float maximo = std::max(abs(lorenz_max_z - lorenz_min_z), abs(lorenz_min_y - lorenz_max_y));
        // Pitch
        Quaternion qz = Quaternion(Vector3(0.0f, 0.0f, 1.0f),
                                   -yoffset * 0.002f); // rotate along Z

        // Roll
        Quaternion qx = Quaternion(Vector3(1, 0, 0), xoffset * 0.002f); // rotate along X

        // Yaw
        Quaternion qy = Quaternion(Vector3(0, 1, 0), xoffset * 0.002f); // rotate along y
        m_Quat = qx * m_Quat;
    }

    m_LastX = (float)xPos;
    m_LastY = (float)yPos;
}

void Trackball::UpdateMouse(GLFWwindow *window)
{
    double xPos;
    double yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    if (MousePressed[GLFW_MOUSE_BUTTON_1] == false && MousePressed[GLFW_MOUSE_BUTTON_2] == false) {
        m_LastX = (float)xPos;
        m_LastY = (float)yPos;
    }

    if (m_FirstMouse) {
        m_LastX = (float)xPos;
        m_LastY = (float)yPos;
        m_FirstMouse = false;
    }

    float xoffset = (float)xPos - m_LastX;
    float yoffset = m_LastY - (float)yPos; // Invertido pois o Y é ao contrário no GLFW

    if (MousePressed[GLFW_MOUSE_BUTTON_1] == true) {
        LoadMaxMin();
        float maximo = std::max(abs(lorenz_max_z - lorenz_min_z), abs(lorenz_min_y - lorenz_max_y));
        // Pitch
        Quaternion qz = Quaternion(Vector3(0.0f, 0.0f, 1.0f),
                                   -yoffset * 0.002f); // rotate along Z

        // Roll
        Quaternion qx = Quaternion(Vector3(1, 0, 0), -yoffset * 0.002f); // rotate along X

        // Yaw
        Quaternion qy = Quaternion(Vector3(0, 1, 0), xoffset * 0.002f); // rotate along y
        m_Quat = qy * qz * m_Quat;
    } else if (MousePressed[GLFW_MOUSE_BUTTON_2] == true) {
        LoadMaxMin();
        float maximo = std::max(abs(lorenz_max_z - lorenz_min_z), abs(lorenz_min_y - lorenz_max_y));
        // Pitch
        Quaternion qz = Quaternion(Vector3(0.0f, 0.0f, 1.0f),
                                   -yoffset * 0.002f); // rotate along Z

        // Roll
        Quaternion qx = Quaternion(Vector3(1, 0, 0), xoffset * 0.002f); // rotate along X

        // Yaw
        Quaternion qy = Quaternion(Vector3(0, 1, 0), xoffset * 0.002f); // rotate along y
        m_Quat = qx * m_Quat;
    }

    m_LastX = (float)xPos;
    m_LastY = (float)yPos;
}

void Perspective::UpdateScroll(float yoffset)
{
    m_Fov -= yoffset;
    m_Fov = std::max(std::min(170.0f, m_Fov), 1.0f);

    m_Projection =
        glm::perspective(glm::radians(m_Fov), (float)screenW / (float)screenH, 0.0010f, 1000.0f);
}

void Ortho::UpdateScroll(float yoffset)
{
    m_Fov -= yoffset * 1.5f;
    m_Fov = std::max(std::min(170.0f, m_Fov), 1.0f);

    LoadMaxMin();
    float maximo = std::max(abs(lorenz_max_z - lorenz_min_z), abs(lorenz_min_y - lorenz_max_y));
    m_Centro = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Position = m_Centro;
    float k = 1.5f * m_Fov / 90.0f;
    m_Projection =
        glm::ortho(-k * maximo, k * maximo, k * maximo, -k * maximo, -200 * maximo, 200 * maximo);
}

void Trackball::UpdateScroll(float yoffset)
{
    m_Fov -= yoffset;
    m_Fov = std::max(std::min(170.0f, m_Fov), 1.0f);

    m_Projection =
        glm::perspective(glm::radians(m_Fov), (float)screenW / (float)screenH, 0.0010f, 1000.0f);
}

glm::mat4 Perspective::GetView()
{

    return glm::lookAt(m_Position, m_Position + m_Target, m_Up);
}

glm::mat4 Ortho::GetView()
{

    glm::vec3 direction = glm::vec3(), eye;
    float maximo = std::max(abs(lorenz_max_z - lorenz_min_z), abs(lorenz_min_y - lorenz_max_y));
    eye = glm::vec3() - (glm::vec3(1.0f, 0.0f, 0.0f) * maximo * 1.5f);

    direction = glm::normalize(direction - eye) + eye;
    glm::vec3 eye_normal = glm::normalize(eye);
    glm::vec3 dir_normal = glm::normalize(direction);

    glm::mat4 quat_mat = glm::make_mat4(m_Quat.getMatrix().get());
    glm::vec3 w_up = glm::vec3(0.0f, -1.0f, 0.0f);

    return glm::lookAt(direction, eye, w_up) * quat_mat;
}

glm::mat4 Trackball::GetView()
{

    glm::vec3 direction = glm::vec3(), eye;
    float maximo = std::max(abs(lorenz_max_z - lorenz_min_z), abs(lorenz_min_y - lorenz_max_y));
    eye = glm::vec3() - (glm::vec3(1.0f, 0.0f, 0.0f) * maximo * 1.5f);

    direction = glm::normalize(direction - eye) + eye;
    glm::vec3 eye_normal = glm::normalize(eye);
    glm::vec3 dir_normal = glm::normalize(direction);

    glm::mat4 quat_mat = glm::make_mat4(m_Quat.getMatrix().get());
    glm::vec3 w_up = glm::vec3(0.0f, 1.0f, 0.0f);

    return glm::lookAt(eye, direction, w_up) * quat_mat;
}
