#pragma once

#include <Raying.h>

namespace Raying {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Raying::Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Raying::Event& e) override;

	private:
		Raying::OrthographicCameraController _cameraCtrl;

		Raying::Ref<Raying::Framebuffer> _fbo;
		Raying::Ref<Raying::Shader> _shader;
		Raying::Ref<Raying::VertexArray> _vao;

		Raying::Ref<Raying::Texture2D> _texture;

		bool _viewportFocused = false, _viewportHovered = false;

		glm::vec2 _viewportSize = { 0.0f, 0.0f };

		glm::vec4 _color = { 0.2f, 0.3f, 0.4f, 1.0f };
	};
}