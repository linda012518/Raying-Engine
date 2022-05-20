#include "hzpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Raying {

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		_orthographicSize = size;
		_orthographicNear = nearClip;
		_orthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		_aspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		float left = -_orthographicSize * _aspectRatio * 0.5f;
		float right = _orthographicSize * _aspectRatio * 0.5f;
		float bottom = -_orthographicSize * 0.5f;
		float top = _orthographicSize * 0.5f;

		_projection = glm::ortho(left, right, bottom, top, _orthographicNear, _orthographicFar);
	}

}