#pragma once

#include "Raying/Core/Core.h"
#include "Raying/Renderer/OrthographicCamera.h"
#include "Raying/Core/Timestep.h"

#include "Raying/Events/ApplicationEvent.h"
#include "Raying/Events/MouseEvent.h"

namespace Raying {

	class Raying_API OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspecRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return _camera; }
		const OrthographicCamera& GetCamera() const { return _camera; }

		float GetZoomLevel() const { return _zoomLevel; }
		void SetZoomLevel(float level) { _zoomLevel = level; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float _aspecRatio;
		float _zoomLevel = 1.0f;
		OrthographicCamera _camera;

		bool _rotation;

		glm::vec3 _cameraPosition = { 0.0f, 0.0f, 0.0f };
		float _cameraRotation = 0.0f;
		float _cameraTranslationSpeed = 5.0f, _cameraRotationSpeed = 180.0f;

	};

}