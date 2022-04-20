#pragma once

#include "Core.h"
#include "Renderer/OrthographicCamera.h"
#include "Core/Timestep.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

namespace Raying {

	class Raying_API OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspecRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return _camera; }
		const OrthographicCamera& GetCamera() const { return _camera; }

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
