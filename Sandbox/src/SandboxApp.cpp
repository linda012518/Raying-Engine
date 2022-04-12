
#include <Raying.h>

#include "imgui/imgui.h"

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

		std::shared_ptr<Raying::VertexBuffer> vbo;
		vbo.reset(Raying::VertexBuffer::Create(vertices, sizeof(vertices)));
		Raying::BufferLayout layout = {
			{Raying::ShaderAttribute::Color, Raying::ShaderDataType::Float4},
			{Raying::ShaderAttribute::Position, Raying::ShaderDataType::Float3}
		};
		vbo->SetLayout(layout);
		_vao->AddVertexBuffer(vbo);

		unsigned int indices[3] = { 0, 1, 2 };
		std::shared_ptr<Raying::IndexBuffer> ibo;
		ibo.reset(Raying::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		_vao->SetIndexBuffer(ibo);


		_blue_vao.reset(Raying::VertexArray::Create());
		float blue[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.70f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		std::shared_ptr<Raying::VertexBuffer> vbo2;
		vbo2.reset(Raying::VertexBuffer::Create(blue, sizeof(blue)));
		Raying::BufferLayout layout2 = {
			{Raying::ShaderAttribute::Position, Raying::ShaderDataType::Float3}
		};
		vbo2->SetLayout(layout2);
		_blue_vao->AddVertexBuffer(vbo2);

		unsigned int blueIndex[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Raying::IndexBuffer> ibo2;
		ibo2.reset(Raying::IndexBuffer::Create(blueIndex, sizeof(blueIndex) / sizeof(uint32_t)));
		_blue_vao->SetIndexBuffer(ibo2);


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 _ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position	= a_Position;
				v_Color		= a_Color;
				gl_Position = _ViewProjection * vec4(a_Position, 1.0);	
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

		_shader.reset(new Raying::Shader(vertexSrc, fragmentSrc));



		std::string blue_vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 _ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position	= a_Position;
				gl_Position = _ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string blue_fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		_blueShader.reset(new Raying::Shader(blue_vertexSrc, blue_fragmentSrc));

	}

	void OnUpdate() override
	{
		//if (Raying::Input::IsKeyPressed(RAYING_KEY_LEFT))
		//	_cameraPosition.x -= _cameraMoveSpeed;
		//else if (Raying::Input::IsKeyPressed(RAYING_KEY_RIGHT))
		//	_cameraPosition.x += _cameraMoveSpeed;

		//if (Raying::Input::IsKeyPressed(RAYING_KEY_UP))
		//	_cameraPosition.y += _cameraMoveSpeed;
		//else if (Raying::Input::IsKeyPressed(RAYING_KEY_DOWN))
		//	_cameraPosition.y -= _cameraMoveSpeed;

		//if (Raying::Input::IsKeyPressed(RAYING_KEY_A))
		//	_cameraRotation += _cameraRotationSpeed;
		//if (Raying::Input::IsKeyPressed(RAYING_KEY_D))
		//	_cameraRotation -= _cameraRotationSpeed;

		Raying::RendererCommand::SetClearColor({ 0, 0.3f, 0, 1 });
		Raying::RendererCommand::Clear();

		_camera.SetPosition(_cameraPosition);
		_camera.SetRotation(_cameraRotation);

		Raying::Renderer::BeginScene(_camera);

		Raying::Renderer::Submit(_blueShader, _blue_vao);
		Raying::Renderer::Submit(_shader, _vao);

		Raying::Renderer::EndScene();

	}

	void OnImGuiRender() override
	{

	}

	void OnEvent(Raying::Event& event) override
	{
		Raying::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Raying::KeyPressedEvent>(Raying_Bind_Event_Fn(ExampleLayer::OnKeyPressed));
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

	std::shared_ptr<Raying::Shader> _shader;
	std::shared_ptr<Raying::VertexArray> _vao;

	std::shared_ptr<Raying::Shader> _blueShader;
	std::shared_ptr<Raying::VertexArray> _blue_vao;

	glm::vec3 _cameraPosition = {0.0f, 0.0f, 0.0f};
	float _cameraMoveSpeed = 0.1f;

	float _cameraRotation = 0.0f;
	float _cameraRotationSpeed = 1.0f;
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
