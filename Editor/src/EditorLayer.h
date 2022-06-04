#pragma once

#include <Raying.h>
#include "Panels/SceneHierarchyPanel.h"
#include "Raying/Renderer/EditorCamera.h"

namespace Raying {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveScene();
		void SaveSceneAs();

	private:
		OrthographicCameraController _cameraCtrl;

		Ref<Framebuffer> _fbo;
		Ref<Shader> _shader;
		Ref<VertexArray> _vao;

		Ref<Scene> _activeScene;
		Entity _squareEntity;
		Entity _cameraEntity;
		Entity _secondCameraEntity;
		Entity _hoverdEntity;

		bool _primoryCamera = true;

		EditorCamera _editorCamera;

		Ref<Texture2D> _texture;

		bool _viewportFocused = false, _viewportHovered = false;

		glm::vec2 _viewportSize = { 0.0f, 0.0f };

		glm::vec2 _viewportBounds[2];

		glm::vec4 _color = { 0.2f, 0.3f, 0.4f, 1.0f };

		int _gizmoType = -1;

		SceneHierarchyPanel _sceneHierarchyPanel;

		std::string _currentScenePath = std::string();
	};
}
