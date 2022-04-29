#include "ExampleLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer::ExampleLayer()
	: Layer("Example"), _cameraCtrl(1280.0f / 720.0f, true)
{
	_vao = Raying::VertexArray::Create();

	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
	};

	Raying::Ref<Raying::VertexBuffer> vbo = Raying::VertexBuffer::Create(vertices, sizeof(vertices));
	Raying::BufferLayout layout = {
		{Raying::ShaderAttribute::Position, Raying::ShaderDataType::Float3},
		{Raying::ShaderAttribute::Color, Raying::ShaderDataType::Float4}
	};
	vbo->SetLayout(layout);
	_vao->AddVertexBuffer(vbo);

	unsigned int indices[3] = { 0, 1, 2 };
	Raying::Ref<Raying::IndexBuffer> ibo = Raying::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	_vao->SetIndexBuffer(ibo);


	_blue_vao = Raying::VertexArray::Create();
	float blue[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};
	Raying::Ref<Raying::VertexBuffer> vbo2 = Raying::VertexBuffer::Create(blue, sizeof(blue));
	Raying::BufferLayout layout2 = {
		{Raying::ShaderAttribute::Position, Raying::ShaderDataType::Float3},
		{Raying::ShaderAttribute::UV1, Raying::ShaderDataType::Float2}
	};
	vbo2->SetLayout(layout2);
	_blue_vao->AddVertexBuffer(vbo2);

	unsigned int blueIndex[6] = { 0, 1, 2, 2, 3, 0 };
	Raying::Ref<Raying::IndexBuffer> ibo2 = Raying::IndexBuffer::Create(blueIndex, sizeof(blueIndex) / sizeof(uint32_t));
	_blue_vao->SetIndexBuffer(ibo2);


	std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 _ViewProjection;
			uniform mat4 _Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position	= a_Position;
				v_Color		= a_Color;
				gl_Position = _ViewProjection * _Transform * vec4(a_Position, 1.0);	
			}
		)";

	std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

	_shader = Raying::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);



	std::string blue_vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 _ViewProjection;
			uniform mat4 _Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position	= a_Position;
				gl_Position = _ViewProjection * _Transform * vec4(a_Position, 1.0);	
			}
		)";

	std::string blue_fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

	_blueShader = Raying::Shader::Create("blue", blue_vertexSrc, blue_fragmentSrc);

	auto textureShader = _shaderLibrary.Load("assets/shaders/Texture.glsl");

	_texture = Raying::Texture2D::Create("assets/textures/Checkerboard.png");
	_logoTexture = Raying::Texture2D::Create("assets/textures/Logo.png");
	textureShader->Bind();
	textureShader->SetInt("u_Texture", 0);
}

void ExampleLayer::OnAttach()
{
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(Raying::Timestep ts)
{
	_cameraCtrl.OnUpdate(ts);

	Raying::RendererCommand::SetClearColor({ 0, 0.3f, 0, 1 });
	Raying::RendererCommand::Clear();

	Raying::Renderer::BeginScene(_cameraCtrl.GetCamera());

	_blueShader->Bind();
	_blueShader->SetFloat3("u_Color", _color);

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	for (size_t y = 0; y < 20; y++)
	{
		for (size_t x = 0; x < 20; x++)
		{
			glm::vec3 pos = glm::vec3(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Raying::Renderer::Submit(_blueShader, _blue_vao, transform);
		}
	}
	//Raying::Renderer::Submit(_shader, _vao);

	auto textureShader = _shaderLibrary.Get("Texture");

	_texture->Bind();
	Raying::Renderer::Submit(textureShader, _blue_vao, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	_logoTexture->Bind();
	Raying::Renderer::Submit(textureShader, _blue_vao, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Raying::Renderer::EndScene();

}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Setting");

	ImGui::ColorEdit3("Box Color", glm::value_ptr(_color));

	ImGui::End();
}

void ExampleLayer::OnEvent(Raying::Event& event)
{
	_cameraCtrl.OnEvent(event);
	//Raying::EventDispatcher dispatcher(event);
	//dispatcher.Dispatch<Raying::KeyPressedEvent>(Raying_Bind_Event_Fn(ExampleLayer::OnKeyPressed));
}

bool ExampleLayer::OnKeyPressed(Raying::KeyPressedEvent& event)
{
	//if (event.GetKeyCode() == RAYING_KEY_LEFT)
	//	_cameraPosition.x -= _cameraMoveSpeed;
	//else if (event.GetKeyCode() == RAYING_KEY_RIGHT)
	//	_cameraPosition.x += _cameraMoveSpeed;

	//if (event.GetKeyCode() == RAYING_KEY_UP)
	//	_cameraPosition.y += _cameraMoveSpeed;
	//else if (event.GetKeyCode() == RAYING_KEY_DOWN)
	//	_cameraPosition.y -= _cameraMoveSpeed;

	return false;
}
