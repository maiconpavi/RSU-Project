#include "window.hpp"

#include <SDL2/SDL.h>
#include <quaternion/Matrices.h>

#include <camera/camera.hpp>
#include <iostream>
#include <solution/solution.hpp>
#include <sstream>
#include <vector>
extern Camera *camera_inuse;

void scroll_callback(GLFWwindow *, double xoffset, double yoffset)
{
    if (send_scroll && !disable_mouse)
        camera_inuse->UpdateScroll((float)yoffset);

    else if (!send_scroll)
        camera_inuse->UpdateScroll((float)0);
}

void window_focus_callback(GLFWwindow *window, int focused)
{
    if (focused && !already_focused) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        already_focused = true;
        window_is_focused = true;
        disable_mouse = false;
    }

    else if (!focused && already_focused) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        already_focused = false;
        window_is_focused = false;
        disable_mouse = true;
    }
}

void mouse_callback(GLFWwindow *, int button, int action, int /*mods*/)
{
    if (action == GLFW_PRESS && button >= 0 && button < 3) {
        MousePressed[button] = true;
    }
    if (action == GLFW_RELEASE && button >= 0 && button < 3) {
        MousePressed[button] = false;
    }
}

CustomWindow::CustomWindow()
{
    if (!glfwInit())
        std::cout << "Falha ao inicializar o GLFW.\n";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_REFRESH_RATE, 60);
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(SDL_GL_MULTISAMPLEBUFFERS, 2);
    glfwWindowHint(SDL_GL_MULTISAMPLESAMPLES, 8);

    this->window = glfwCreateWindow(screenW, screenH, "Lorenz Attractor", NULL, NULL);
    if (this->window == NULL)
        std::cout << "Erro ao criar a janela.\n";

    glfwSetScrollCallback(this->window, scroll_callback);
    glfwSetWindowFocusCallback(this->window, window_focus_callback);
    glfwSetMouseButtonCallback(this->window, mouse_callback);

    glfwMakeContextCurrent(this->window);
    glfwMaximizeWindow(this->window);
    glfwSwapInterval(GLFW_TRUE);
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void CustomWindow::set_input_mode(int cursor, int mode)
{
    glfwSetInputMode(main_window->window, cursor, mode);
}

bool CustomWindow::tl_key_toggle(int key)
{
    if (this->keys_pressed.find(key) == this->keys_pressed.end()) {
        this->keys_pressed[key] = false;
    }

    if (this->keys_toggle_latch.find(key) == this->keys_toggle_latch.end()) {
        this->keys_toggle_latch[key] = false;
    }

    if (this->keys_state_changed.find(key) == this->keys_state_changed.end()) {
        this->keys_state_changed[key] = false;
    }

    this->keys_state_changed[key] = false;

    if ((glfwGetKey(main_window->get_window(), GLFW_KEY_ESCAPE) == GLFW_RELEASE) && (this->keys_pressed[key])) {
        this->keys_pressed[key] = false;
    }

    if ((glfwGetKey(main_window->get_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS) && !this->keys_pressed[key] && !this->keys_toggle_latch[key]) {
        this->keys_pressed[key] = true;
        this->keys_toggle_latch[key] = true;
        this->keys_state_changed[key] = true;

    } else if ((glfwGetKey(main_window->get_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS) && !this->keys_pressed[key] && this->keys_toggle_latch[key]) {
        this->keys_pressed[key] = true;
        this->keys_toggle_latch[key] = false;
        this->keys_state_changed[key] = true;
    }

    return this->keys_state_changed[key];
}

bool CustomWindow::tl_key_is_on(int key)
{
    return this->keys_toggle_latch[key];
}

void CustomWindow::tl_set_key(int key, bool value)
{
    this->keys_toggle_latch[key] = value;
}
