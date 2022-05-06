#include "hzpch.h"
#include "OrthographicCameraController.h"

#include "Raying/Core/Input.h"
#include "Raying/Core/KeyCodes.h"

namespace Raying {

	OrthographicCameraController::OrthographicCameraController(float aspecRatio, bool rotation)
		: _aspecRatio(aspecRatio), m_Bounds({ -_aspecRatio * _zoomLevel, _aspecRatio * _zoomLevel, -_zoomLevel, _zoomLevel }), 
		_camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top), _rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		Raying_Profile_FUNCTION();

		if (Input::IsKeyPressed(RAYING_KEY_A))
		{
			_cameraPosition.x -= cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
			_cameraPosition.y -= sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(RAYING_KEY_D))
		{
			_cameraPosition.x += cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
			_cameraPosition.y += sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
		}

		if (Input::IsKeyPressed(RAYING_KEY_W))
		{
			_cameraPosition.x += -sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
			_cameraPosition.y += cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(RAYING_KEY_S))
		{
			_cameraPosition.x -= -sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
			_cameraPosition.y -= cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
		}

		if (_rotation)
		{
			if (Input::IsKeyPressed(RAYING_KEY_Q))
				_cameraRotation += _cameraRotationSpeed * ts;
			if (Input::IsKeyPressed(RAYING_KEY_E))
				_cameraRotation -= _cameraRotationSpeed * ts;

			if (_cameraRotation > 180.0f)
				_cameraRotation -= 360.0f;
			else if (_cameraRotation <= -180.0f)
				_cameraRotation += 360.0f;

			_camera.SetRotation(_cameraRotation);
		}

		_camera.SetPosition(_cameraPosition);

		_cameraTranslationSpeed = _zoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event & e)
	{
		Raying_Profile_FUNCTION();

		EventDispatcher dispathcer(e);
		dispathcer.Dispatch<MouseScrolledEvent>(Raying_Bind_Event_Fn(OrthographicCameraController::OnMouseScrolled));
		dispathcer.Dispatch<WindowResizeEvent>(Raying_Bind_Event_Fn(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent & e)
	{
		Raying_Profile_FUNCTION();

		_zoomLevel -= e.GetYOffset() *0.25f;
		_zoomLevel = std::max(_zoomLevel, 0.25f);

		m_Bounds = { -_aspecRatio * _zoomLevel, _aspecRatio * _zoomLevel, -_zoomLevel, _zoomLevel };

		_camera.SetPorjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent & e)
	{
		Raying_Profile_FUNCTION();

		_aspecRatio = (float)e.GetWidth() / (float)e.GetHeight();

		m_Bounds = { -_aspecRatio * _zoomLevel, _aspecRatio * _zoomLevel, -_zoomLevel, _zoomLevel };

		_camera.SetPorjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);

		return false;
	}

}