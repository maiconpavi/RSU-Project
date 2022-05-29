#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include <quaternion/Quaternion.h>


extern unsigned int screenW;
extern unsigned int screenH;
extern bool MousePressed[2];
extern float lorenz_max_x, lorenz_max_y, lorenz_max_z;
extern float lorenz_min_x, lorenz_min_y, lorenz_min_z;

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 pyr, float fov);

	virtual glm::mat4 GetView()  = 0;
	virtual void UpdateMouse(GLFWwindow *window) = 0;
	virtual void UpdateScroll(float yoffset) = 0;
	virtual void UpdateKeyboard(GLFWwindow *window, float dt) = 0;

	virtual void SetLeftCamera() = 0;
	virtual void SetFrontCamera() = 0;
	virtual void SetTopCamera() = 0;

	void UpdateVectors();
	void SetPYR(glm::vec3 pyr);
	void SetLastMousePos(glm::vec2 last);
	void SetPosition(glm::vec3 pos);
	void SetTarget(glm::vec3 tar);

	glm::vec3 GetPosition()
	{
		return m_Position;
	}

	glm::vec3 GetTarget()
	{
		return m_Target;
	}

	glm::mat4 GetProjection()
	{
		return m_Projection;
	}

	float GetPitch()
	{
		return m_Pitch;
	}
	float GetYaw()
	{
		return m_Yaw;
	}

	float GetRoll()
	{
		return m_Roll;
	}

	float GetFov()
	{
		return m_Fov;
	}

	int Type()
	{
		return m_Type;
	}

	bool GetPerspective()
	{
		return m_Ortho;
	}

protected:
	glm::mat4 m_View;
	glm::mat4 m_Projection;

	glm::vec3 m_Position;
	glm::vec3 m_Target;
	glm::vec3 m_Right;
	glm::vec3 m_Up;

	float speed_up = 1.0f;
	bool pressed = false;
	bool m_Ortho = false;

	float m_Yaw = 0.0f;
	float m_Pitch = 0.0f;
	float m_Roll = 0.0f;

	float m_lYaw = 0.0f;
	float m_lPitch = 0.0f;
	float m_lRoll = 0.0f;

	float m_Fov = 90.0f;

	float m_Speed = 8.0f;
	float r_Speed = 40.0f;
	float boost = 2.0f;
	float m_Sensitivity = 0.4f;

	float m_LastX = screenW / 2.0f;
	float m_LastY = screenH / 2.0f;
	bool m_FirstMouse = true;

	int m_Type;
};

class Perspective : public Camera
{
public:
	Perspective(glm::vec3 position, glm::vec3 pyr, float fov);

	virtual glm::mat4 GetView() override;
	virtual void UpdateMouse(GLFWwindow *window) override;
	virtual void UpdateScroll(float yoffset) override;
	virtual void UpdateKeyboard(GLFWwindow *window, float dt) override;

	virtual void SetLeftCamera() override;
	virtual void SetFrontCamera() override;
	virtual void SetTopCamera() override;
};

class Ortho : public Camera
{
public:
	Ortho(glm::vec3 position, glm::vec3 pyr, float fov);

	virtual glm::mat4 GetView() override;
	virtual void UpdateMouse(GLFWwindow *window) override;
	virtual void UpdateScroll(float yoffset) override;
	virtual void UpdateKeyboard(GLFWwindow *window, float dt) override;

	virtual void SetLeftCamera() override{};
	virtual void SetFrontCamera() override{};
	virtual void SetTopCamera() override{};

private:
	glm::vec3 m_Centro;
	Quaternion m_Quat;
};

class Trackball : public Camera
{
public:
	Trackball(glm::vec3 position, glm::vec3 pyr, float fov);

	virtual glm::mat4 GetView() override;
	virtual void UpdateMouse(GLFWwindow *window) override;
	virtual void UpdateScroll(float yoffset) override;
	virtual void UpdateKeyboard(GLFWwindow *window, float dt) override;

	virtual void SetLeftCamera() override{};
	virtual void SetFrontCamera() override{};
	virtual void SetTopCamera() override{};

private:
	Quaternion m_Quat;
};

#endif
