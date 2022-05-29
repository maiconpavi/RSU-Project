// System includes
#include "mainmenu.hpp"

#include <ctype.h> // toupper
#include <limits.h> // INT_MIN, INT_MAX
#include <math.h> // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h> // vsnprintf, sscanf, printf
#include <stdlib.h> // NULL, malloc, free, atoi

#include <string>

#ifdef _WIN32
#	define IM_NEWLINE "\r\n"
#else
#	define IM_NEWLINE "\n"
#endif

#define IM_MIN(A, B) (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B) (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX) ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

// Enforce cdecl calling convention for functions called by the standard library, in case compilation settings changed the default to e.g. __vectorcall
#ifndef IMGUI_CDECL
#	ifdef _MSC_VER
#		define IMGUI_CDECL __cdecl
#	else
#		define IMGUI_CDECL
#	endif
#endif

using namespace std;

void ShowOverlay(bool* p_open)
{
	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();
	glm::vec3 position = camera_inuse->GetPosition();
	glm::vec3 target = camera_inuse->GetTarget();
	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
	if(corner != -1)
	{
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if(ImGui::Begin("Camera Overlay", p_open, window_flags))
	{
		ImGui::Text("Informações da câmera:");

		ImGui::Separator();
		ImGui::Text("Position: (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
		ImGui::Separator();
		ImGui::Text("Target: (%.2f, %.2f, %.2f)", target.x, target.y, target.z);
		ImGui::Separator();
		ImGui::Text("PYR: %.2f, %.2f, %.2f",
					camera_inuse->GetPitch(),
					camera_inuse->GetYaw(),
					camera_inuse->GetRoll());
		ImGui::Separator();
		ImGui::Text("FOV: %.2f", camera_inuse->GetFov()); //, Roll: %.2f°
		ImGui::Separator();
		bool isortho = camera_inuse->GetPerspective();
		if(isortho)
			ImGui::Text("Ortográfica");
		else
			ImGui::Text("Perspectiva");

		if(ImGui::BeginPopupContextWindow())
		{
			// if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
			if(ImGui::MenuItem("Top-left", NULL, corner == 0))
				corner = 0;
			if(ImGui::MenuItem("Top-right", NULL, corner == 1))
				corner = 1;
			if(ImGui::MenuItem("Bottom-left", NULL, corner == 2))
				corner = 2;
			if(ImGui::MenuItem("Bottom-right", NULL, corner == 3))
				corner = 3;
			if(p_open && ImGui::MenuItem("Close"))
				*p_open = false;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void AnimationOverlay(bool* p_open)
{
	static int corner = 1;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
	if(corner != -1)
	{
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if(ImGui::Begin("Consts Overlay", p_open, window_flags))
	{
		ImGui::Text("Constantes:");

		ImGui::Separator();
		ImGui::Text("Alpha: %.2f", constantes[0]);
		ImGui::Spacing();
		ImGui::Text("Beta: %.2f", constantes[1]);
		ImGui::Spacing();
		ImGui::Text("Rho: %.2f", constantes[2]);
		ImGui::Spacing();

		if(ImGui::BeginPopupContextWindow())
		{
			// if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
			if(ImGui::MenuItem("Top-left", NULL, corner == 0))
				corner = 0;
			if(ImGui::MenuItem("Top-right", NULL, corner == 1))
				corner = 1;
			if(ImGui::MenuItem("Bottom-left", NULL, corner == 2))
				corner = 2;
			if(ImGui::MenuItem("Bottom-right", NULL, corner == 3))
				corner = 3;
			if(p_open && ImGui::MenuItem("Close"))
				*p_open = false;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if(ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void TextCentered(std::string text, bool indented, int pos)
{
	auto windowWidth = ImGui::GetWindowSize().x;
	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
	auto indentWidth = ImGui::GetStyle().IndentSpacing;

	if(indented)
	{
		ImGui::SetCursorPosX((indentWidth + windowWidth - textWidth) * 0.5f);
		//ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.5 - 0.5f * textWidth);
	}
	else
	{
		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	}

	ImGui::Text(text.c_str());
}

bool SliderFloatDefault(const char* label,
						float* v,
						float v_min,
						float v_max,
						float v_default,
						const char* display_format,
						ImGuiSliderFlags sflags)
{
	bool ret = ImGui::SliderFloat(label, v, v_min, v_max, display_format, sflags);
	if(ImGui::BeginPopupContextItem(label))
	{
		char buf[64];
		sprintf(buf, "Resetar para %.3f", v_default);
		if(ImGui::MenuItem(buf))
			*v = v_default;
		ImGui::MenuItem("Fechar");
		ImGui::EndPopup();
	}
	return ret;
}

MainMenu::MainMenu(ImVec2 size)
	: m_Size(size)
{ }

bool MainMenu::autosave_is_on()
{
	return m_Auto_Save;
}

bool MainMenu::camera_overlay_is_on()
{
	return m_ShowAppCameraInfo;
}

void MainMenu::set_camera_overlay_state(bool isopen)
{
	m_ShowAppCameraInfo = isopen;
}

void MainMenu::set_start(bool isfirst)
{
	m_IsFirst = isfirst;
}

void MainMenu::close_main_menu(){
	main_window->tl_set_key(GLFW_KEY_ESCAPE, false);

	double x, y;
	glfwGetCursorPos(main_window->get_window(), &x, &y);
	camera_inuse->SetLastMousePos(glm::vec2(x, y));

	main_window->set_input_mode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	this->set_start(true);
	send_scroll = true;
}

void MainMenu::main_loop(bool* menu_is_open)
{
	// Set Global Variable
	IsAnimeted = { m_aAlpha, m_aBeta, m_aRho };

	MinMaxAlpha = { m_vAlpha[0], m_vAlpha[1] };
	MinMaxBeta = { m_vBeta[0], m_vBeta[1] };
	MinMaxRho = { m_vRho[0], m_vRho[1] };
	AnimationSpeed = { m_sAlpha, m_sBeta, m_sRho };

	// Set Min/Max for SliderFloat

	float min1[2] = { 0.0f, m_vAlpha[0] }, max1 =  50.0f;
	float min2[2] = { 0.0f, m_vBeta[0] }, max2 = 10.0f ;
	float min3[2] = { 0.0f, m_vRho[0] }, max3 = 50.0f;

	if(m_ShowAppMetrics)
	{
		ImGui::ShowMetricsWindow(&m_ShowAppMetrics);
	}

	if(m_ShowAppStackTool)
	{
		ImGui::ShowStackToolWindow(&m_ShowAppStackTool);
	}
	if(m_ShowAppStyleEditor)
	{
		ImGui::Begin("Menu de edição de estilo", &m_ShowAppStyleEditor);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

	if(m_Auto_Save)
	{
		if(!m_aAlpha)
		{
			constantes[0] = m_TempConst[0];
		}
		if(!m_aBeta)
		{
			constantes[1] = m_TempConst[1];
		}
		if(!m_aRho)
		{
			constantes[2] = m_TempConst[2];
		}
		color_c1 = m_TempColor1;
		color_c2 = m_TempColor2;
		back_color = m_TempBackColor;
		cor_prop = m_TempCorProp;
	}

	if(m_IsFirst && !m_SavePosition)
	{
		ImGui::SetNextWindowPos(
			ImVec2(screenW / 2, screenH / 2), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		m_IsFirst = false;
	}

	ImGui::SetNextWindowSize(m_Size, ImGuiCond_FirstUseEver);

	if(!ImGui::Begin("Menu Principal", menu_is_open, m_Window_Flags))
	{
		// ImGui::End();
		// return;
	}

	float SB_Size = ImGui::GetStyle().ScrollbarSize;
	auto indentWidth = ImGui::GetStyle().IndentSpacing;

	ImGuiIO& io = ImGui::GetIO();

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	if(ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("Ferramentas"))
		{
			ImGui::MenuItem("Métricas/Debugger", NULL, &m_ShowAppMetrics);
			ImGui::MenuItem("Ferramenta de Stack", NULL, &m_ShowAppStackTool);
			ImGui::MenuItem("Menu de edição de estilo", NULL, &m_ShowAppStyleEditor);
			ImGui::MenuItem("Informações da Câmera", NULL, &m_ShowAppCameraInfo); //m_ShowAppCameraInfo
			ImGui::MenuItem("Valor das constantes", NULL, &cov_isopen);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	//ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	ImGui::BeginChild(
		"ChildL", ImVec2(ImGui::GetContentRegionAvail().x, m_Size.y - m_Size.y / 10), false);
	ImGui::Columns(2, "mainc", false);
	ImGui::SetColumnWidth(0, m_Size.x - m_ItemSpacing - SB_Size);

	if(ImGui::CollapsingHeader("Configurações gerais"))
	{
		ImGui::Indent();

		ImGui::Checkbox("Aplicar automaticamente", &m_Auto_Save);
		static bool selection[5] = { false, true, false, false, false };
		ImGui::Spacing();
		ImGui::Separator();

		ImGui::SetNextItemWidth(m_Size.x - m_ItemSpacing * 2);
		ImGui::PushItemWidth(-(SB_Size + m_ItemSpacing));
		if(ImGui::Selectable("Paleta de cores em forma de barra", m_ColorPickerType == 1))
			m_ColorPickerType = 1;

		ImGui::SetNextItemWidth(m_Size.x - m_ItemSpacing * 2);
		if(ImGui::Selectable("Paleta de cores em forma de roda", m_ColorPickerType == 0))
			m_ColorPickerType = 0;

		ImGui::Separator();
		ImGui::Spacing();

		if(m_ColorPickerType == 1)
			ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR |
									   ImGuiColorEditFlags_PickerHueBar);
		if(m_ColorPickerType == 0)
			ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR |
									   ImGuiColorEditFlags_PickerHueWheel);

		ImGui::Checkbox("Salvar posição do menu", &m_SavePosition);

		ImGui::Checkbox("Eixo de cordenadas", &axis_on);

		if(axis_on)
		{
			TextCentered(std::string("Tamanho do eixo"), true);
			ImGui::SetNextItemWidth(m_Size.x - 2 * m_ItemSpacing - SB_Size - indentWidth);
			ImGui::SliderFloat("K", &k, 0.0f, 100.0f);
		}
		ImGui::Spacing();
		ImGui::Checkbox("Ponto Central", &ponto_meio);
		ImGui::Spacing();

		ImGui::Unindent();
	}

	if(ImGui::CollapsingHeader("Configurações das constantes"))
	{
		ImGui::Indent();

		if(!m_aAlpha)
		{
			TextCentered(std::string("Alpha"), true);
			ImGui::SetNextItemWidth(m_Size.x - 2 * m_ItemSpacing - SB_Size - indentWidth);
			SliderFloatDefault("Alpha", &m_TempConst[0], 0.0f, 50.0f, constantes[0]);
			ImGui::Spacing();
		}

		if(!m_aBeta)
		{
			TextCentered(std::string("Beta"), true);
			ImGui::SetNextItemWidth(m_Size.x - 2 * m_ItemSpacing - SB_Size - indentWidth);
			SliderFloatDefault("Beta", &m_TempConst[1], 0.0f, 10.0f, constantes[1]);
			ImGui::Spacing();
		}
		if(!m_aRho)
		{
			TextCentered(std::string("Rho"), true);
			ImGui::SetNextItemWidth(m_Size.x - 2 * m_ItemSpacing - SB_Size - indentWidth);
			SliderFloatDefault("Rho", &m_TempConst[2], 0.0f, 50.0f, constantes[2]);
			ImGui::Spacing();
			ImGui::Spacing();
		}

		ImGui::BeginTable("Table1", 2);
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

		if(ImGui::Button("Resetar constantes",
						 ImVec2(m_Size.x / 2 - m_ItemSpacing * 1.5 - indentWidth / 2, 0.0f)))
		{
			m_TempConst = { 10.0f, 8.0f / 3.0f, 28.0f };
		}

		ImGui::TableSetColumnIndex(1);

		if(!m_Auto_Save)
		{
			if(ImGui::Button("Aplicar",
							 ImVec2(m_Size.x / 2 - m_ItemSpacing * 1.5 - indentWidth / 2, 0.0f)))
			{
				constantes = m_TempConst;

				apply_soulution = true;
			}
		}

		ImGui::EndTable();
		ImGui::Spacing();
		ImGui::Unindent();
	}

	if(ImGui::CollapsingHeader("Animar constantes"))
	{
		ImGui::Indent();

		ImGui::Checkbox("Animar##1", &m_aAlpha);
		ImGui::SameLine();
		TextCentered(std::string("Alpha"), true);
		//////////////////
		ImGui::SetNextItemWidth(m_Size.x - 2 * m_ItemSpacing - SB_Size - indentWidth);

		if(m_vAlpha[0] > m_vAlpha[1])
		{
			m_vAlpha[1] = m_vAlpha[0];
		}
		// ImGui::SliderFloat2M("Alpha##2", m_vAlpha, min1, max1);
		ImGui::DragFloatRange2("Alpha##2", &m_vAlpha[0], &m_vAlpha[1], 0.25f, 0.0f, max1, "Min: %.2f", "Max: %.2f");

		
		//////////////////

		ImGui::SetNextItemWidth(m_Size.x - 2 * m_ItemSpacing - SB_Size - indentWidth);
		ImGui::SliderFloat("a##3", &m_sAlpha, 0.0f, 10.0f);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Checkbox("Animar##2", &m_aBeta);
		ImGui::SameLine();
		TextCentered(std::string("Beta"), true);
		ImGui::SetNextItemWidth(m_Size.x - 2 * m_ItemSpacing - SB_Size - indentWidth);
		//////////////////
		ImGui::SetNextItemWidth(m_Size.x - 2 * m_ItemSpacing - SB_Size - indentWidth);

		if(m_vBeta[0] > m_vBeta[1])
		{
			m_vBeta[1] = m_vBeta[0];
		}
		// ImGui::SliderFloat2M("Beta##2", m_vBeta, min2, max2);
		ImGui::DragFloatRange2("Beta##2", &m_vBeta[0], &m_vBeta[1], 0.25f, 0.0f, max2, "Min: %.2f", "Max: %.2f");

		//////////////////

		ImGui::SetNextItemWidth(m_Size.x - 2 * m_ItemSpacing - SB_Size - indentWidth);
		ImGui::SliderFloat("b##3", &m_sBeta, 0.0f, 10.0f);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Checkbox("Animar##3", &m_aRho);
		ImGui::SameLine();
		TextCentered(std::string("Rho"), true);

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x + 2 * m_ItemSpacing);

		//////////////////
		ImGui::SetNextItemWidth(m_Size.x - 2 * m_ItemSpacing - SB_Size - indentWidth);

		if(m_vRho[0] > m_vRho[1])
		{
			m_vRho[1] = m_vRho[0];
		}
		 // ImGui::SliderFloat2M("Rho##2", m_vRho, min3, max3);
		ImGui::DragFloatRange2("Rho##2", &m_vRho[0], &m_vRho[1], 0.25f, 0.0f, max3, "Min: %.2f", "Max: %.2f");
		//////////////////

		ImGui::SetNextItemWidth(m_Size.x - 2 * m_ItemSpacing - SB_Size - indentWidth);
		ImGui::SliderFloat("r##3", &m_sRho, 0.0f, 10.0f);
		ImGui::Spacing();
		ImGui::Unindent();
	}

	if(ImGui::CollapsingHeader("Configurações das cores"))
	{
		ImGui::Indent();
		TextCentered(std::string("Proporção de cor"), true);
		ImGui::PushItemWidth(m_Size.x - 2 * m_ItemSpacing);
		SliderFloatDefault("m_TempCorProp",
						   &m_TempCorProp,
						   0.0f,
						   5.0f,
						   m_TempCorProp,
						   "%.3f",
						   ImGuiSliderFlags_Logarithmic);
		ImGui::Spacing();

		TextCentered(std::string("Cor 1"), true);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x + m_ItemSpacing - 1);
		ImGui::ColorEdit3("Color Picker1", &m_TempColor1.x);

		TextCentered(std::string("Cor 2"), true);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x + m_ItemSpacing - 1);
		ImGui::ColorEdit3("Color Picker 2", &m_TempColor2.x);
		ImGui::Spacing();

		TextCentered(std::string("Cor de fundo"), true);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x + m_ItemSpacing - 1);
		ImGui::ColorEdit3("Color Picker 3", &m_TempBackColor.x);
		ImGui::Spacing();

		ImGui::BeginTable("Table2", 2);
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

		if(ImGui::Button("Resetar cores",
						 ImVec2(m_Size.x / 2 - m_ItemSpacing * 1.5 - indentWidth / 2, 0.0f)))
		{
			m_TempColor1 = white_c;
			m_TempColor2 = blue_c;
			m_TempCorProp = 1.5f;
			m_TempBackColor = { RGBto1(3.0f), RGBto1(2.0f), RGBto1(3.0f) };
		}

		ImGui::TableSetColumnIndex(1);

		if(!m_Auto_Save)
		{
			if(ImGui::Button("Aplicar##2",
							 ImVec2(m_Size.x / 2 - m_ItemSpacing * 1.5 - indentWidth / 2, 0.0f)))
			{
				color_c1 = m_TempColor1;
				color_c2 = m_TempColor2;
				back_color = m_TempBackColor;
				cor_prop = m_TempCorProp;

				apply_soulution = true;
			}
		}

		ImGui::EndTable();
		ImGui::Unindent();
	}

	if(ImGui::CollapsingHeader("Opções da câmera"))
	{
		ImGui::Indent();
		TextCentered(std::string("Tipos de câmera"), true);
		ImGui::Separator();
		//m_CameraType

		if(ImGui::Selectable("Perspectiva", m_CameraType == 0))
			m_CameraType = 0;
		ImGui::Spacing();

		if(ImGui::Selectable("Ortográfica", m_CameraType == 1))
			m_CameraType = 1;
		ImGui::Spacing();

		if(ImGui::Selectable("Trackball", m_CameraType == 2))
			m_CameraType = 2;

		if(m_LastCameraType != m_CameraType)
		{
			LoadMaxMin();
			if(m_CameraType == 0)
			{
				if(z_co_temp != 0.0f)
				{
					z_co = 0.0f;
					z_co_temp = 0.0f;
				}
				camera_inuse = perspective_c;
				camera_inuse->UpdateScroll(0.0f);
			}
			if(m_CameraType == 1)
			{
				if(z_co_temp == 0.0f)
				{
					z_co = -lorenz_max_z / 2.0f;
					z_co_temp = z_co;
				}
				camera_inuse = ortho_c;
				camera_inuse->UpdateScroll(0.0f);
			}
			if(m_CameraType == 2)
			{
				if(z_co_temp == 0.0f)
				{
					z_co = -lorenz_max_z / 2.0f;
					z_co_temp = z_co;
				}
				camera_inuse = trackball_c;
				camera_inuse->UpdateScroll(0.0f);
			}
			m_LastCameraType = m_CameraType;
		}

		ImGui::Separator();

		if(m_CameraType == 0)
		{
			ImGui::Spacing();
			TextCentered(string(("Vista")), true);

			ImGui::BeginTable("Table3", 3);
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			if(ImGui::Button("Esquerda", ImVec2(m_Size.x / 3, 0.0f)))
			{
				camera_inuse->SetLeftCamera();
			}
			ImGui::TableSetColumnIndex(1);

			if(ImGui::Button("Frente", ImVec2(m_Size.x / 3, 0.0f)))
			{
				camera_inuse->SetFrontCamera();
			}
			ImGui::TableSetColumnIndex(2);

			if(ImGui::Button("Superior", ImVec2(m_Size.x / 3, 0.0f)))
			{
				camera_inuse->SetTopCamera();
			}
			ImGui::EndTable();
		}

		ImGui::Unindent();
	}
	ImGui::Columns(1);
	ImGui::EndChild();

	ImGui::SetCursorPosY(m_Size.y - m_Size.y / 40);
	ImGui::Columns(2, "c0", false);

	if(ImGui::Button("Continuar", ImVec2(m_Size.x / 2, m_Size.y / 20)))
	{
		*menu_is_open = false;
		this->close_main_menu();
		
	}

	ImGui::NextColumn();

	if(ImGui::Button("Sair", ImVec2(m_Size.x / 2, m_Size.y / 20)))
	{
		quit_window = true;
	}

	ImGui::Columns(1);
	ImGui::End();
}
