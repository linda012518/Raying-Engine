#include "hzpch.h"
#include "Application.h"
#include "Log.h"
#include "Renderer/Renderer.h"

#include <glad/glad.h>

namespace Raying
{
	#define Bind_Event_Fn(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::_instance = nullptr;

	Application::Application()
	{
		Raying_Core_Assert(!_instance, "Application alread exists!");
		_instance = this;

		_window = std::unique_ptr<Window>(Window::Create());
		_window->SetEventCallback(Bind_Event_Fn(OnEvent));

		_imguiLayer = new ImGuiLayer();
		PushLayer(_imguiLayer);

		_vao.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vbo;
		vbo.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ShaderAttribute::Color, ShaderDataType::Float4},
			{ShaderAttribute::Position, ShaderDataType::Float3}
		};
		vbo->SetLayout(layout);
		_vao->AddVertexBuffer(vbo);

		unsigned int indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> ibo;
		ibo.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		_vao->SetIndexBuffer(ibo);


		_blue_vao.reset(VertexArray::Create());
		float blue[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.70f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		std::shared_ptr<VertexBuffer> vbo2;
		vbo2.reset(VertexBuffer::Create(blue, sizeof(blue)));
		BufferLayout layout2 = {
			{ShaderAttribute::Position, ShaderDataType::Float3}
		};
		vbo2->SetLayout(layout2);
		_blue_vao->AddVertexBuffer(vbo2);

		unsigned int blueIndex[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> ibo2;
		ibo2.reset(IndexBuffer::Create(blueIndex, sizeof(blueIndex) / sizeof(uint32_t)));
		_blue_vao->SetIndexBuffer(ibo2);


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position	= a_Position;
				v_Color		= a_Color;
				gl_Position = vec4(a_Position, 1.0);	
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

		_shader.reset(new Shader(vertexSrc, fragmentSrc));



		std::string blue_vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position	= a_Position;
				gl_Position = vec4(a_Position, 1.0);	
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

		_blueShader.reset(new Shader(blue_vertexSrc, blue_fragmentSrc));

	}

	void Application::PushLayer(Layer* layer)
	{
		_layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		_layerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(Bind_Event_Fn(OnWindowClose));

		for (auto itr = _layerStack.end(); itr != _layerStack.begin(); )
		{
			(*--itr)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (_running)
		{
			RendererCommand::SetClearColor({ 0, 0.3f, 0, 1 });
			RendererCommand::Clear();

			Renderer::BeginScene();

			_blueShader->Bind();
			Renderer::Submit(_blue_vao);
			_shader->Bind();
			Renderer::Submit(_vao);

			Renderer::EndScene();
			
			for (Layer* layer : _layerStack)
				layer->OnUpdate();

			_imguiLayer->Begin();
			for (Layer* layer : _layerStack)
				layer->OnImGuiRender();
			_imguiLayer->End();

			_window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		_running = false;
		return true;
	}

}
