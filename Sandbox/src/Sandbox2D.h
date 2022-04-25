#pragma once

#include <Raying.h>

class Sandbox2D : public Raying::Layer
{
public:
	Sandbox2D();
	~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Raying::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Raying::Event& e) override;

private:
	Raying::OrthographicCameraController _cameraCtrl;

	Raying::Ref<Raying::Shader> _shader;
	Raying::Ref<Raying::VertexArray> _vao;

	Raying::Ref<Raying::Texture2D> _texture;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> _profileResults;

	glm::vec4 _color = { 0.2f, 0.3f, 0.4f, 1.0f };

};