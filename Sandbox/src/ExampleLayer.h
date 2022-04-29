#pragma once

#include "Raying.h"

class ExampleLayer : public Raying::Layer
{
public:
	ExampleLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual ~ExampleLayer() = default;

	void OnUpdate(Raying::Timestep ts) override;

	void OnImGuiRender() override;

	void OnEvent(Raying::Event& event) override;

	bool OnKeyPressed(Raying::KeyPressedEvent& event);

private:
	Raying::OrthographicCameraController _cameraCtrl;

	Raying::ShaderLibrary _shaderLibrary;

	Raying::Ref<Raying::Shader> _shader;
	Raying::Ref<Raying::VertexArray> _vao;

	Raying::Ref<Raying::Shader> _blueShader;
	Raying::Ref<Raying::VertexArray> _blue_vao;

	Raying::Ref<Raying::Texture> _texture, _logoTexture;

	glm::vec3 _color;
};

