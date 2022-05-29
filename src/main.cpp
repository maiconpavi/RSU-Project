#include <glad/glad.h>
#include <limits.h>
#include <stdio.h>

#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
// Include nos Vendors

#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//
//	*** Macros ***
//

#define BIT_OFFSET_F(n) (const void *)(n * sizeof(float))

#define RGBto1(n) (const float)((float)n / 255.0f)

//
//	*** Constantes Globais ***
//
//  Inserir a resolução da tela nas constantes abaixo
//

unsigned int screenW = 1920;
unsigned int screenH = 1080;
const char *glsl_version = "#version 130";

bool apply_soulution = false;
bool quit_window = false;
bool axis_on = true;
bool ponto_meio = true;
bool cov_isopen = false;

bool MousePressed[2] = {false, false};

std::array<bool, 3> IsAnimeted = {false, false, false};
std::array<float, 3> AnimationSpeed;
std::array<float, 2> MinMaxAlpha;
std::array<float, 2> MinMaxBeta;
std::array<float, 2> MinMaxRho;

float lorenz_max_x, lorenz_max_y, lorenz_max_z;
float lorenz_min_x, lorenz_min_y, lorenz_min_z;
float z_co = 0.0f;
float z_co_temp = 0.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 color1 = {RGBto1(255), RGBto1(255), RGBto1(255)};
glm::vec3 color2 = {RGBto1(0), RGBto1(0), RGBto1(255)};
glm::vec3 back_color = {RGBto1(3.0f), RGBto1(2.0f), RGBto1(3.0f)};

glm::vec3 constantes = {10.0f, 8.0f / 3.0f, 28.0f};
float cor_prop = 1.5f;

float k = 20.0f;

extern glm::vec3 color_c1, color_c2;

#define white_c          \
    {                    \
        1.0f, 1.0f, 1.0f \
    }
#define blue_c           \
    {                    \
        0.0f, 0.0f, 1.0f \
    }

glm::vec3 color_c1 = white_c, color_c2 = blue_c;

#define _USE_MATH_DEFINES
// Define para o getcwd
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif
#define GetCurrentDir getcwd

// #define GLUT_DISABLE_ATEXIT_HACK
// #include <GL/freeglut.h>

#include <camera/camera.hpp>
#include <mainmenu/mainmenu.hpp>
#include <shader/shader.hpp>
#include <solution/solution.hpp>
#include <window/window.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Imgui
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// SDL
#include <SDL2/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

using namespace std;

std::array<Point3D, (const size_t)number_of_points> solution;

//
//	*** Variáveis Globais ***
//

CustomWindow *main_window;

Camera *camera_inuse;
Camera *perspective_c = new Perspective(glm::vec3(), glm::vec3(0.0f, 90.0f, 0.0f), 90.0f);

Camera *ortho_c = new Ortho(glm::vec3(), glm::vec3(0.0f, 0.0f, 180.0f), 90.0f);

Camera *trackball_c = new Trackball(glm::vec3(), glm::vec3(), 90.0f);

// glm::vec3(0.0f, 180.0f, 0.0f)

double cursor_lastx, cursor_lasty;

bool send_scroll = true;
bool window_is_focused = true;
bool already_focused = true;
bool disable_mouse = false;

extern std::array<Point3D, number_of_points> lorenzSolution;
extern const int number_of_points;

// x x/4 x/3 x/4
const float line_length = 1.0f;

float pontos[108] = {

    /*

        pontos[6 * 1] = k;
        pontos[6 * 7 + 1] = k;
        pontos[6 * 13 + 2] = k;
        */
    // x axis
    -4.0,
    0.0f,
    0.0f,
    1.0f,
    0.0f,
    0.0f,

    4.0,
    0.0f,
    0.0f,
    1.0f,
    0.0f,
    0.0f,

    // arrow
    4.0,
    0.0f,
    0.0f,
    1.0f,
    0.0f,
    0.0f,

    3.5,
    0.5f,
    0.0f,
    1.0f,
    0.0f,
    0.0f, //-1 -2

    4.0,
    0.0f,
    0.0f,
    1.0f,
    0.0f,
    0.0f,

    3.5,
    -0.5f,
    0.0f,
    1.0f,
    0.0f,
    0.0f, // -1 -4

    // y axis
    0.0f,
    -4.0f,
    0.0f,
    0.0f,
    1.0f,
    0.0f,

    0.0f,
    4.0f,
    0.0f,
    0.0f,
    1.0f,
    0.0f,

    // arrow
    0.0f,
    4.0f,
    0.0f,
    0.0f,
    1.0f,
    0.0f,

    0.5f,
    3.5f,
    0.0f,
    0.0f,
    1.0f,
    0.0f,

    0.0f,
    4.0f,
    0.0f,
    0.0f,
    1.0f,
    0.0f,

    -0.5f,
    3.5f,
    0.0f,
    0.0f,
    1.0f,
    0.0f,

    // z axis
    0.0f,
    0.0f,
    -4.0f,
    0.0f,
    0.0f,
    1.0f,

    0.0f,
    0.0f,
    4.0f,
    0.0f,
    0.0f,
    1.0f,

    // arrow
    0.0f,
    0.0f,
    4.0f,
    0.0f,
    0.0f,
    1.0f,

    0.0f,
    0.5f,
    3.5f,
    0.0f,
    0.0f,
    1.0f,

    0.0f,
    0.0f,
    4.0f,
    0.0f,
    0.0f,
    1.0f,

    0.0f,
    -0.5f,
    3.5f,
    0.0f,
    0.0f,
    1.0f

};

//
//	*** Prototipagem de Funções ***
//

float last_zp = 0.0f;

void ChangeZPoints()
{
    float delta = z_co - last_zp;

    if (delta != 0.0f) {
        for (int i = 0; i < 18; i++) {
            pontos[6 * i + 2] += delta;
        }
        last_zp = z_co;
    }
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void window_focus_callback(GLFWwindow *window, int focused);

void mouse_callback(GLFWwindow *, int button, int action, int /*mods*/);

bool LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char *image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE); // This is required on WebGL for non
    // power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 image_width,
                 image_height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

void initialize_solution_buffer(GLuint *lorenzVAO, GLuint *lorenzVBO)
{
    glGenVertexArrays(1, lorenzVAO);
    glGenBuffers(1, lorenzVBO);

    glBindVertexArray(*lorenzVAO);

    glBindBuffer(GL_ARRAY_BUFFER, *lorenzVBO);
    glBufferStorage(GL_ARRAY_BUFFER,
                    sizeof(Point3D) * solution.size(),
                    solution.data(),
                    GL_DYNAMIC_STORAGE_BIT);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point3D), BIT_OFFSET_F(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point3D), BIT_OFFSET_F(3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void update_solution_buffer(GLuint *lorenzVAO, GLuint *lorenzVBO)
{
    glBindBuffer(GL_ARRAY_BUFFER, *lorenzVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Point3D) * solution.size(), solution.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

float DistanceB(glm::vec3 point1, glm::vec3 point2)
{
    glm::vec3 delta = point1 - point2;

    return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

std::vector<Point3D> LineDivide(glm::vec3 point1, glm::vec3 point2, glm::vec3 l_color)
{

    float space = line_length / 3.0f;
    float long_line = line_length, short_line = line_length / 4.0f;
    float total = long_line + space + short_line + space;
    int i = 0;

    glm::vec3 delta = point2 - point1;

    float dist = DistanceB(point1, point2);
    glm::vec3 normal = glm::normalize(delta);

    std::vector<Point3D> pontos_divididos;
    std::vector<float> divisoes = {long_line, space, short_line, space};

    float size = 0;

    while (size < dist) {
        for (i = 0; (i < divisoes.size()) && (size < dist); i++) {
            if (divisoes[i] != space) {
                pontos_divididos.push_back(Point3D(point1 + size * normal, l_color));
            }
            size += divisoes[i];
            if (divisoes[i] != space) {
                pontos_divididos.push_back(Point3D(point1 + size * normal, l_color));
            }
        }
    }
    if (divisoes[i] == space) {
        pontos_divididos.pop_back();
        pontos_divididos.push_back(Point3D(point2, l_color));
    }

    return pontos_divididos;
}

void UpdateArrows()
{
    pontos[6 * 2] = pontos[6 * 1];
    pontos[6 * 4] = pontos[6 * 1];

    pontos[6 * 3] = pontos[6 * 1] - 0.5f;
    pontos[6 * 5] = pontos[6 * 1] - 0.5f;

    ///////////////////////////////////////

    pontos[6 * 8 + 1] = pontos[6 * 7 + 1];
    pontos[6 * 10 + 1] = pontos[6 * 7 + 1];

    pontos[6 * 9 + 1] = pontos[6 * 7 + 1] - 0.5f;
    pontos[6 * 11 + 1] = pontos[6 * 7 + 1] - 0.5f;

    ///////////////////////////////////////

    pontos[6 * 14 + 2] = pontos[6 * 13 + 2];
    pontos[6 * 16 + 2] = pontos[6 * 13 + 2];

    pontos[6 * 15 + 2] = pontos[6 * 13 + 2] - 0.5f;
    pontos[6 * 17 + 2] = pontos[6 * 13 + 2] - 0.5f;
}

int SetAxis(GLuint *axisVAO, GLuint *axisVBO)
{
    pontos[6 * 1] = k;
    pontos[6 * 7 + 1] = k;
    pontos[6 * 13 + 2] = k;
    UpdateArrows();

    glDeleteVertexArrays(1, axisVAO);
    glDeleteBuffers(1, axisVBO);

    glGenVertexArrays(1, axisVAO);
    glGenBuffers(1, axisVBO);

    glBindVertexArray(*axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, *axisVBO);

    std::vector<Point3D> pontos_v;

    for (int i = 0; i < 18; i++) {
        pontos_v.push_back(
            Point3D(glm::vec3(pontos[6 * i + 0], pontos[6 * i + 1], pontos[6 * i + 2]),
                    glm::vec3(pontos[6 * i + 3], pontos[6 * i + 4], pontos[6 * i + 5])));
    }

    std::vector<Point3D> pontos_vd;
    for (int i = 0; i < pontos_v.size() / 2; i++) {
        std::vector<Point3D> temp = LineDivide(
            pontos_v[2 * i].Position, pontos_v[2 * i + 1].Position, pontos_v[2 * i].Color);
        pontos_vd.insert(pontos_vd.end(), temp.begin(), temp.end());
    }

    glBufferStorage(GL_ARRAY_BUFFER,
                    sizeof(Point3D) * pontos_vd.size(),
                    pontos_vd.data(),
                    GL_DYNAMIC_STORAGE_BIT);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point3D), BIT_OFFSET_F(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point3D), BIT_OFFSET_F(3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return pontos_vd.size();
}

int main(int argc, char **argv)
{

    // Efetua o Calculo dos pontos do Attractor

    solution = computeLorenz();

    // Inicialização do GLFW

    // Criação do contexto Gráfico e da Janela

    main_window = new CustomWindow();

    // Imgui
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplGlfw_InitForOpenGL(main_window->get_window(), true))
        std::cout << "Falha ao inicializar o ImGui_ImplGlfw.\n";
    if (!ImGui_ImplOpenGL3_Init(glsl_version))
        std::cout << "Falha ao inicializar o ImplOpenGL3.\n";

    MainMenu main_menu = MainMenu(ImVec2(screenW / 4, screenH / 1.5));

    // Captura o Cursor

    // Carregamento dos ponteiros de funções do OpenGL usando o GLAD

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cout << "Falha ao inicializar o GLAD.\n";

    // Criação do Shader

    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    }

    std::string shaders_folder = std::string(cwd);
    // shaders_folder.erase(shaders_folder.find("/src") + 1, 3);
    shaders_folder += "/shaders";

    Shader *defaultShader = new Shader(shaders_folder);

    defaultShader->CreateShader((const char *)(shaders_folder + "/default.vert").c_str(),
                                GL_VERTEX_SHADER);
    defaultShader->CreateShader((const char *)(shaders_folder + "/default.frag").c_str(),
                                GL_FRAGMENT_SHADER);

    camera_inuse = perspective_c;

    // Ativação do Teste de profundidade para renderizar em 3D e aumenta o tamanho
    // do ponto do OpenGL

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glPointSize(10.0f);

    // Criação da VAO e VBO do triângulo

    float vertices[12] = {// Ponto Atual
                          0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f

    };

    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    if (ponto_meio) {
        glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_STORAGE_BIT);
    } else {
        glBufferStorage(
            GL_ARRAY_BUFFER, (int)(sizeof(vertices) / 2), vertices, GL_DYNAMIC_STORAGE_BIT);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point3D), BIT_OFFSET_F(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point3D), BIT_OFFSET_F(3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Criação da VAO e VBO do atractor de lorenz

    GLuint lorenzVAO; //= (GLuint *)malloc(sizeof(unsigned int *))
    GLuint lorenzVBO;

    GLuint axisVAO;
    GLuint axisVBO;

    initialize_solution_buffer(&lorenzVAO, &lorenzVBO);

    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);

    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);

    std::vector<Point3D> pontos_v;

    for (int i = 0; i < 18; i++) {
        pontos_v.push_back(
            Point3D(glm::vec3(pontos[6 * i + 0], pontos[6 * i + 1], pontos[6 * i + 2]),
                    glm::vec3(pontos[6 * i + 3], pontos[6 * i + 4], pontos[6 * i + 5])));
    }

    std::vector<Point3D> pontos_vd;
    for (int i = 0; i < pontos_v.size() / 2; i++) {
        std::vector<Point3D> temp = LineDivide(
            pontos_v[2 * i].Position, pontos_v[2 * i + 1].Position, pontos_v[2 * i].Color);
        pontos_vd.insert(pontos_vd.end(), temp.begin(), temp.end());
    }

    glBufferStorage(GL_ARRAY_BUFFER,
                    sizeof(Point3D) * pontos_vd.size(),
                    pontos_vd.data(),
                    GL_DYNAMIC_STORAGE_BIT);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point3D), BIT_OFFSET_F(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point3D), BIT_OFFSET_F(3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Entrada do loop principal da apliação

    // Variável de controle do ponto atual
    size_t currentIterationStep = 0;
    glm::mat4 pv;

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool pressed = false;
    bool mainmenu_is_open = false;
    bool first_time = true;
    bool open_already = false;

    bool selection_live = true;
    bool temp_b = false;

    const float ItemSpacing = ImGui::GetStyle().ItemSpacing.x;

    double lastx, lasty;
    float last_z = z_co;
    int k_a = 1, k_b = 1, k_r = 1;

    glm::vec3 last_pos = camera_inuse->GetPosition(), last_tar = camera_inuse->GetTarget();

    glm::vec3 temp_pos, temp_tar;

    // Imagem da equação
    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    int number_of_p3d = 0;

    string image_path = string(cwd) + string("/img.png");
    bool ret = LoadTextureFromFile(
        image_path.c_str(), &my_image_texture, &my_image_width, &my_image_height);

    // glfwGetCursorPos(main_window->get_window(), &cursor_lastx, &cursor_lasty);
    camera_inuse->SetLeftCamera();

    float temp_k = k;
    number_of_p3d = SetAxis(&axisVAO, &axisVAO);

    std::cout << "Entrando no loop principal" << '\n';
    while (!glfwWindowShouldClose(main_window->get_window())) {
        // Cálculo do deltaTime do frame

        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Animate Const's

        if (IsAnimeted[0]) {
            float proximo = constantes[0] + (float)k_a * AnimationSpeed[0] * deltaTime;
            if (proximo > MinMaxAlpha[1]) {
                k_a = -1;
            }
            if (proximo < MinMaxAlpha[0]) {
                k_a = 1;
            }
            constantes[0] += (float)k_a * AnimationSpeed[0] * deltaTime;
        }

        if (IsAnimeted[1]) {
            float proximo = constantes[1] + (float)k_a * AnimationSpeed[1] * deltaTime;
            if (proximo > MinMaxBeta[1]) {
                k_a = -1;
            }
            if (proximo < MinMaxBeta[0]) {
                k_a = 1;
            }
            constantes[1] += (float)k_a * AnimationSpeed[1] * deltaTime;
        }

        if (IsAnimeted[2]) {
            float proximo = constantes[2] + (float)k_a * AnimationSpeed[2] * deltaTime;
            if (proximo > MinMaxRho[1]) {
                k_a = -1;
            }
            if (proximo < MinMaxRho[0]) {
                k_a = 1;
            }
            constantes[2] += (float)k_a * AnimationSpeed[2] * deltaTime;
        }
        
        // Novo frame do Imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Cálculo e atualização das coordenadas do ponto atual

        float currentPoint[6] = {

            // Posição do ponto
            solution[currentIterationStep].Position.x,
            solution[currentIterationStep].Position.y,
            solution[currentIterationStep].Position.z,

            RGBto1(255),
            RGBto1(255),
            RGBto1(255)};

        if (currentIterationStep < number_of_points)
            currentIterationStep++;
        else
            currentIterationStep = 0;

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Point3D), currentPoint);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Movimentação da Câmera
        if (!mainmenu_is_open) {
            camera_inuse->UpdateKeyboard(main_window->get_window(), deltaTime);
            if (!disable_mouse)
                camera_inuse->UpdateMouse(main_window->get_window());
        }

        // Preparação do Render
        glClearColor(back_color.r, back_color.g, back_color.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render

        defaultShader->Use();
        // if (camera_changed) {
        //     main_window->set_input_mode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        //     camera_changed = false;
        // }
        pv = camera_inuse->GetProjection() * camera_inuse->GetView();
        defaultShader->SetMat4("pv", pv);

        // Draw Points
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 1);
        if (ponto_meio)
            glDrawArrays(GL_POINTS, 1, 1);
        glBindVertexArray(0);

        // Draw Lorenz Atractor
        glBindVertexArray(lorenzVAO);

        glLineWidth(1.0f);
        glDrawArrays(GL_LINE_STRIP, 0, number_of_points);
        glBindVertexArray(0);

        LoadMaxMin();
        int camera_type = camera_inuse->Type();

        float k_error1 = 0.01f, k_error2 = 0.02f;
        switch (camera_type) {
        case 0: { // "Perspective"

            z_co = 0.0f;
            z_co_temp = z_co;
            break;
        }
        case 1: { // "Ortho"
            LoadMaxMin();
            float prox = -abs(lorenz_max_z - lorenz_min_z) / 2.0f;
            if ((prox > (z_co + k_error1 * z_co)) || (prox < (z_co - k_error1 * z_co))) {
                z_co = prox;
                z_co_temp = z_co;
            }

            break;
        }
        case 2: { // "Trackball"
            LoadMaxMin();
            float prox = -abs(lorenz_max_z - lorenz_min_z) / 2.0f;
            if ((prox > (z_co + k_error1 * z_co)) || (prox < (z_co - k_error1 * z_co))) {
                z_co = prox;
                z_co_temp = z_co;
            }
            break;
        }
        }

        // Draw Axis
        if (axis_on) {
            glBindVertexArray(axisVAO);
            glDrawArrays(GL_LINES, 0, number_of_p3d);
            glBindVertexArray(0);
        }

        defaultShader->UnUse();

        

		if (!window_is_focused && !mainmenu_is_open) {
			send_scroll = false;            
			mainmenu_is_open = true;
			main_window->tl_set_key(GLFW_KEY_ESCAPE, true);

		}

		if (main_window->tl_key_toggle(GLFW_KEY_ESCAPE)){
			if ((main_window->tl_key_is_on(GLFW_KEY_ESCAPE))) {
				send_scroll = false;            
				mainmenu_is_open = true;
			} else {
				main_window->set_input_mode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				mainmenu_is_open = false;
				main_menu.close_main_menu();
			}

		}

        if (mainmenu_is_open) {
            main_window->set_input_mode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            open_already = true;
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

			mainmenu_is_open = true;
            main_menu.main_loop(&mainmenu_is_open);
        }


        

        if (cov_isopen)
        {
            AnimationOverlay(&cov_isopen);
        }
		
        if (main_menu.camera_overlay_is_on()) {
			bool camera_is_open = true;
            ShowOverlay(&camera_is_open);
            if (!camera_is_open) {
                main_menu.set_camera_overlay_state(false);
            }
        }
        if (temp_k != k || last_z != z_co) {
            ChangeZPoints();
            number_of_p3d = SetAxis(&axisVAO, &axisVBO);
            temp_k = k;
            apply_soulution = true;
            last_z = z_co;
        }

        if (main_menu.autosave_is_on() || apply_soulution) {
            solution = computeLorenz(constantes[0], constantes[1], constantes[2], cor_prop);
            update_solution_buffer(&lorenzVAO, &lorenzVBO);

            apply_soulution = false;
        }

        

        if (quit_window) {
            glfwSetWindowShouldClose(main_window->get_window(), GLFW_TRUE);
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(main_window->get_window(), &display_w, &display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Troca dos Buffers e captura de eventos do OS
        glfwSwapBuffers(main_window->get_window());

        glfwPollEvents();
    }

    // Destruição das VAO's e VBO's

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &axisVAO);
    glDeleteBuffers(1, &axisVBO);

    glDeleteVertexArrays(1, &lorenzVAO);
    glDeleteBuffers(1, &lorenzVBO);

    // Destruição do shader

    delete defaultShader;
    defaultShader = nullptr;

    // Destruição da Janela e finalização do GLFW

    glfwDestroyWindow(main_window->get_window());
    glfwTerminate();

    return 0;
}

// Callbacks do GLFW
