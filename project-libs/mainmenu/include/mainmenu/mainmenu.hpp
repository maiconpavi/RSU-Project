#pragma once
#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include <solution/solution.hpp>
#include <camera/camera.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <window/window.hpp>

#define white_c          \
    {                    \
        1.0f, 1.0f, 1.0f \
    }
#define blue_c           \
    {                    \
        0.0f, 0.0f, 1.0f \
    }
#define RGBto1(n) (const float)((float)n / 255.0f)

extern bool apply_soulution;
extern bool quit_window;
extern bool axis_on;
extern bool ponto_meio;
extern bool cov_isopen;

extern unsigned int screenW;
extern unsigned int screenH;

extern CustomWindow *main_window;

extern glm::vec3 constantes;
extern glm::vec3 back_color;
extern glm::vec3 color_c1, color_c2;
extern float cor_prop;

extern float lorenz_max_x, lorenz_max_y, lorenz_max_z;
extern float lorenz_min_x, lorenz_min_y, lorenz_min_z;
extern float z_co;
extern float deltaTime;
extern Camera *camera_inuse;
extern Camera *perspective_c;
extern Camera *ortho_c;
extern Camera *trackball_c;

extern std::array<bool, 3> IsAnimeted;
extern std::array<float, 3> AnimationSpeed;
extern std::array<float, 2> MinMaxAlpha;
extern std::array<float, 2> MinMaxBeta;
extern std::array<float, 2> MinMaxRho;

extern float z_co_temp;

extern float k;

void ShowOverlay(bool *p_open);
void AnimationOverlay(bool *p_open);
void TextCentered(std::string text, bool indented = false, int pos = 2);
bool SliderFloatDefault(const char *label,
                        float *v,
                        float v_min,
                        float v_max,
                        float v_default,
                        const char *display_format = "%.3f",
                        ImGuiSliderFlags sflags = 0);

class MainMenu
{
public:
    MainMenu(ImVec2 size);

    void main_loop(bool *p_open);
    bool autosave_is_on();
    void set_start(bool isfirst);
    bool camera_overlay_is_on();
    void set_camera_overlay_state(bool isopen);
	void close_main_menu();

private:
    ImGuiWindowFlags m_Window_Flags =
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize;

    bool m_Auto_Save = true;
    bool m_IsFirst = true;
    bool m_SavePosition = false;

    ImVec2 m_Size;

    glm::vec3 m_TempConst = constantes;
    glm::vec3 m_TempColor1 = color_c1;
    glm::vec3 m_TempColor2 = color_c2;
    glm::vec3 m_TempBackColor = back_color;

    float m_TempCorProp = cor_prop;

    bool m_ShowAppMetrics = false;
    bool m_ShowAppStackTool = false;
    bool m_ShowAppStyleEditor = false;
    bool m_ShowAppCameraInfo = true;
    bool m_aAlpha = false;
    bool m_aBeta = false;
    bool m_aRho = false;

    int m_ColorPickerType = 0;

    int m_CameraType = 0;
    int m_LastCameraType = 0;

    float m_vAlpha[2] = {0.0f, 50.0f};
    float m_vBeta[2] = {0.0f, 10.0f};
    float m_vRho[2] = {0.0f, 50.0f};

    float m_sAlpha = 2.0f;
    float m_sBeta = 2.0f;
    float m_sRho = 2.0f;

    int m_kAlpha = 1;
    int m_kBeta = 1;
    int m_kRho = 1;

    float m_lZ = 0;

    const float m_ItemSpacing = ImGui::GetStyle().ItemSpacing.x;
};

#endif
