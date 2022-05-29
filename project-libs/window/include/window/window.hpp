#pragma once
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include <SDL2/SDL.h>

#include <iostream>
#include <map>
#include <quaternion/Quaternion.h>
#include <string>
#include <string_view>

extern unsigned int screenW;
extern unsigned int screenH;

extern bool MousePressed[2];
extern float lorenz_max_x, lorenz_max_y, lorenz_max_z;
extern float lorenz_min_x, lorenz_min_y, lorenz_min_z;
extern const char *glsl_version;

extern bool send_scroll;
extern bool window_is_focused;
extern bool already_focused;
extern bool disable_mouse;

class CustomWindow
{
public:
    CustomWindow();

    static CustomWindow &getInstance()
    {
        static CustomWindow instance;
        return instance;
    }
    GLFWwindow *get_window()
    {
        return this->window;
    }

    bool tl_key_is_on(int key);
    bool tl_key_toggle(int key);
    void tl_set_key(int key, bool value);
    void set_input_mode(int cursor, int mode);

protected:
    std::map<int, bool> keys_toggle_latch = {};
    std::map<int, bool> keys_pressed = {};
    std::map<int, bool> keys_state_changed = {};
    GLFWwindow *window;
};

extern CustomWindow *main_window;

#endif
