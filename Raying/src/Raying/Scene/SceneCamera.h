#pragma once

#include "Raying/Renderer/Camera.h"

namespace Raying {

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFov, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetPerspectiveFOV() const { return _perspectiveFOV; }
		void SetPerspectiveFOV(float fov) { _perspectiveFOV = fov; RecalculateProjection(); }
		float GetPerspectiveNearClip() const { return _perspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { _perspectiveNear = nearClip; RecalculateProjection(); }
		float GetPerspectiveFarClip() const { return _perspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { _perspectiveFar = farClip; RecalculateProjection(); }

		float GetOrthographicSize() const { return _orthographicSize; }
		void SetOrthographicSize(float size) { _orthographicSize = size; RecalculateProjection(); }

		float GetOrthographicNearClip() const { return _orthographicNear; }
		void SetOrthographicNearClip(float nearClip) { _orthographicNear = nearClip; RecalculateProjection(); }
		float GetOrthographicFarClip() const { return _orthographicFar; }
		void SetOrthographicFarClip(float farClip) { _orthographicFar = farClip; RecalculateProjection(); }

		ProjectionType GetProjectionType() const { return _projectionType; }
		void SetProjectionType(ProjectionType type) { _projectionType = type; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		ProjectionType _projectionType = ProjectionType::Orthographic;

		float _perspectiveFOV = glm::radians(45.0f);
		float _perspectiveNear = 0.01f, _perspectiveFar = 1000.0f;

		float _orthographicSize = 10.0f;
		float _orthographicNear = -1.0f, _orthographicFar = 1.0f;

		float _aspectRatio = 0.0f;

	};

}
