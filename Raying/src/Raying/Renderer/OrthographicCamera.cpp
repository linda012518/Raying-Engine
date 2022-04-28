#include "hzpch.h"
#include "OrthographicCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Raying {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: _projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), _viewMatrix(glm::mat4(1.0f))
	{
		Raying_Profile_FUNCTION();

		_vpMatrix = _projectionMatrix * _viewMatrix;
	}

	void OrthographicCamera::SetPorjection(float left, float right, float bottom, float top)
	{
		Raying_Profile_FUNCTION();

		_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		_vpMatrix = _projectionMatrix * _viewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()	
	{
		Raying_Profile_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), glm::vec3(0, 0, 1));

		_viewMatrix = glm::inverse(transform);
		_vpMatrix = _projectionMatrix * _viewMatrix;
	}

}