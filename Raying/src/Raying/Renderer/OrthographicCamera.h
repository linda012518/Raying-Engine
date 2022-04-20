#pragma once

#include "Raying/Core.h"

#include <glm/glm.hpp>

namespace Raying {

	class Raying_API OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetPorjection(float left, float right, float bottom, float top);

		const glm::vec3& GetPostion() const { return _position; }
		void SetPosition(const glm::vec3& position) { _position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return _rotation; }
		void SetRotation(float rotation) { _rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return _projectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return _viewMatrix; }
		const glm::mat4& GetVPMatrix() const { return _vpMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 _projectionMatrix;
		glm::mat4 _viewMatrix;
		glm::mat4 _vpMatrix;

		glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
		float _rotation = 0;

	};

}