#include "hzpch.h"
#include "OrthographicCameraController.h"

#include "Raying/Core/Input.h"
#include "Raying/Core/KeyCodes.h"

namespace Raying {

	OrthographicCameraController::OrthographicCameraController(float aspecRatio, bool rotation)
		: _aspecRatio(aspecRatio), _camera(-_aspecRatio * _zoomLevel, _aspecRatio * _zoomLevel, -_zoomLevel, _zoomLevel), _rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(RAYING_KEY_A))
			_cameraPosition.x -= _cameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(RAYING_KEY_D))
			_cameraPosition.x += _cameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(RAYING_KEY_W))
			_cameraPosition.y += _cameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(RAYING_KEY_S))
			_cameraPosition.y -= _cameraTranslationSpeed * ts;

		if (_rotation)
		{
			if (Input::IsKeyPressed(RAYING_KEY_Q))
				_cameraRotation += _cameraRotationSpeed * ts;
			if (Input::IsKeyPressed(RAYING_KEY_E))
				_cameraRotation -= _cameraRotationSpeed * ts;

			_camera.SetRotation(_cameraRotation);
		}

		_camera.SetPosition(_cameraPosition);

		_cameraTranslationSpeed = _zoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event & e)
	{
		EventDispatcher dispathcer(e);
		dispathcer.Dispatch<MouseScrolledEvent>(Raying_Bind_Event_Fn(OrthographicCameraController::OnMouseScrolled));
		dispathcer.Dispatch<WindowResizeEvent>(Raying_Bind_Event_Fn(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent & e)
	{
		_zoomLevel -= e.GetYOffset() *0.25f;
		_zoomLevel = std::max(_zoomLevel, 0.25f);

		_camera.SetPorjection(-_aspecRatio * _zoomLevel, _aspecRatio * _zoomLevel, -_zoomLevel, _zoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent & e)
	{
		_aspecRatio = (float)e.GetWidth() / (float)e.GetHeight();
		_camera.SetPorjection(-_aspecRatio * _zoomLevel, _aspecRatio * _zoomLevel, -_zoomLevel, _zoomLevel);

		return false;
	}

}