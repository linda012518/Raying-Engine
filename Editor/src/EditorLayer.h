#pragma once

#include <Raying.h>
#include "Panels/SceneHierarchyPanel.h"

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
		OrthographicCameraController _cameraCtrl;

		Ref<Framebuffer> _fbo;
		Ref<Shader> _shader;
		Ref<VertexArray> _vao;

		Ref<Scene> _activeScene;
		Entity _squareEntity;
		Entity _cameraEntity;
		Entity _secondCameraEntity;

		bool _primoryCamera = true;

		Ref<Texture2D> _texture;

		bool _viewportFocused = false, _viewportHovered = false;

		glm::vec2 _viewportSize = { 0.0f, 0.0f };

		glm::vec4 _color = { 0.2f, 0.3f, 0.4f, 1.0f };

		SceneHierarchyPanel _sceneHierarchyPanel;
	};
}
