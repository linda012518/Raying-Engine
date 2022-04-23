#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), _cameraCtrl(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	_vao = Raying::VertexArray::Create();

	float squareVertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Raying::Ref<Raying::VertexBuffer> vbo;
	vbo.reset(Raying::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	vbo->SetLayout({
		{Raying::ShaderAttribute::Position, Raying::ShaderDataType::Float3}
		});
	_vao->AddVertexBuffer(vbo);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Raying::Ref<Raying::IndexBuffer> ibo;
	ibo.reset(Raying::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	_vao->SetIndexBuffer(ibo);

	_shader = Raying::Shader::Create("assets/shaders/FlatColor.glsl");

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Raying::Timestep ts)
{
	// Update
	_cameraCtrl.OnUpdate(ts);

	// Render
	Raying::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Raying::RendererCommand::Clear();

	Raying::Renderer::BeginScene(_cameraCtrl.GetCamera());

	std::dynamic_pointer_cast<Raying::OpenGLShader>(_shader)->Bind();
	std::dynamic_pointer_cast<Raying::OpenGLShader>(_shader)->UploadUniformFloat4("u_Color", _color);

	Raying::Renderer::Submit(_shader, _vao, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Raying::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(_color));
	ImGui::End();
}

void Sandbox2D::OnEvent(Raying::Event & e)
{
	_cameraCtrl.OnEvent(e);
}

