
#include <Raying.h>
#include <Platform/OpenGL/OpenGLShader.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Raying::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), _camera(-1.8f, 1.8f, -1.0f, 1.0f)
	{
		_vao.reset(Raying::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		Raying::Ref<Raying::VertexBuffer> vbo;
		vbo.reset(Raying::VertexBuffer::Create(vertices, sizeof(vertices)));
		Raying::BufferLayout layout = {
			{Raying::ShaderAttribute::Position, Raying::ShaderDataType::Float3},
			{Raying::ShaderAttribute::Color, Raying::ShaderDataType::Float4}
		};
		vbo->SetLayout(layout);
		_vao->AddVertexBuffer(vbo);

		unsigned int indices[3] = { 0, 1, 2 };
		Raying::Ref<Raying::IndexBuffer> ibo;
		ibo.reset(Raying::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		_vao->SetIndexBuffer(ibo);


		_blue_vao.reset(Raying::VertexArray::Create());
		float blue[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		Raying::Ref<Raying::VertexBuffer> vbo2;
		vbo2.reset(Raying::VertexBuffer::Create(blue, sizeof(blue)));
		Raying::BufferLayout layout2 = {
			{Raying::ShaderAttribute::Position, Raying::ShaderDataType::Float3},
			{Raying::ShaderAttribute::UV1, Raying::ShaderDataType::Float2}
		};
		vbo2->SetLayout(layout2);
		_blue_vao->AddVertexBuffer(vbo2);

		unsigned int blueIndex[6] = { 0, 1, 2, 2, 3, 0 };
		Raying::Ref<Raying::IndexBuffer> ibo2;
		ibo2.reset(Raying::IndexBuffer::Create(blueIndex, sizeof(blueIndex) / sizeof(uint32_t)));
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

		_shader.reset(Raying::Shader::Create(vertexSrc, fragmentSrc));



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

		_blueShader.reset(Raying::Shader::Create(blue_vertexSrc, blue_fragmentSrc));


		std::string texture_vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_Texcoord;

			uniform mat4 _ViewProjection;
			uniform mat4 _Transform;

			out vec2 v_Texcoord;

			void main()
			{
				v_Texcoord	= a_Texcoord;
				gl_Position = _ViewProjection * _Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string texture_fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_Texcoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_Texcoord);
				//color = vec4(v_Texcoord, 0.0, 1.0);
			}
		)";

		_textureShader.reset(Raying::Shader::Create(texture_vertexSrc, texture_fragmentSrc));

		_texture = Raying::Texture2D::Create("assets/textures/Checkerboard.png");
		std::dynamic_pointer_cast<Raying::OpenGLShader>(_textureShader)->Bind();
		std::dynamic_pointer_cast<Raying::OpenGLShader>(_textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Raying::Timestep ts) override
	{
		if (Raying::Input::IsKeyPressed(RAYING_KEY_LEFT))
			_cameraPosition.x -= _cameraMoveSpeed * ts;
		else if (Raying::Input::IsKeyPressed(RAYING_KEY_RIGHT))
			_cameraPosition.x += _cameraMoveSpeed * ts;

		if (Raying::Input::IsKeyPressed(RAYING_KEY_UP))
			_cameraPosition.y += _cameraMoveSpeed * ts;
		else if (Raying::Input::IsKeyPressed(RAYING_KEY_DOWN))
			_cameraPosition.y -= _cameraMoveSpeed * ts;

		if (Raying::Input::IsKeyPressed(RAYING_KEY_A))
			_cameraRotation += _cameraRotationSpeed * ts;
		if (Raying::Input::IsKeyPressed(RAYING_KEY_D))
			_cameraRotation -= _cameraRotationSpeed * ts;

		Raying::RendererCommand::SetClearColor({ 0, 0.3f, 0, 1 });
		Raying::RendererCommand::Clear();

		_camera.SetPosition(_cameraPosition);
		_camera.SetRotation(_cameraRotation);

		Raying::Renderer::BeginScene(_camera);

		std::dynamic_pointer_cast<Raying::OpenGLShader>(_blueShader)->Bind();
		std::dynamic_pointer_cast<Raying::OpenGLShader>(_blueShader)->UploadUniformFloat3("u_Color", _color);

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
		Raying::Renderer::Submit(_shader, _vao);

		_texture->Bind();
		Raying::Renderer::Submit(_textureShader, _blue_vao, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Raying::Renderer::EndScene();

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Setting");

		ImGui::ColorEdit3("Box Color", glm::value_ptr(_color));

		ImGui::End();
	}

	void OnEvent(Raying::Event& event) override
	{
		//Raying::EventDispatcher dispatcher(event);
		//dispatcher.Dispatch<Raying::KeyPressedEvent>(Raying_Bind_Event_Fn(ExampleLayer::OnKeyPressed));
	}

	bool OnKeyPressed(Raying::KeyPressedEvent& event)
	{
		if (event.GetKeyCode() == RAYING_KEY_LEFT)
			_cameraPosition.x -= _cameraMoveSpeed;
		else if (event.GetKeyCode() == RAYING_KEY_RIGHT)
			_cameraPosition.x += _cameraMoveSpeed;

		if (event.GetKeyCode() == RAYING_KEY_UP)
			_cameraPosition.y += _cameraMoveSpeed;
		else if (event.GetKeyCode() == RAYING_KEY_DOWN)
			_cameraPosition.y -= _cameraMoveSpeed;

		return false;
	}

private:
	Raying::OrthographicCamera _camera;

	Raying::Ref<Raying::Shader> _shader;
	Raying::Ref<Raying::VertexArray> _vao;

	Raying::Ref<Raying::Shader> _blueShader;
	Raying::Ref<Raying::VertexArray> _blue_vao;

	Raying::Ref<Raying::Shader> _textureShader;
	Raying::Ref<Raying::Texture> _texture;

	glm::vec3 _cameraPosition = {0.0f, 0.0f, 0.0f};
	float _cameraMoveSpeed = 5.0f;

	float _cameraRotation = 0.0f;
	float _cameraRotationSpeed = 180.0f;

	glm::vec3 _color;
};

class Sandbox : public Raying::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}

};

Raying::Application* Raying::CreateApplication()
{
	return new Sandbox();
}
