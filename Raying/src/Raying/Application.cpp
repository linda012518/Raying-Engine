#include "hzpch.h"
#include "Application.h"

#include "Log.h"

#include <glad/glad.h>

namespace Raying
{
	#define Bind_Event_Fn(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::_instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
		}

		Raying_Core_Assert(false, "Unknown ShaderDataType!");
		return 0;
	}

	Application::Application()
	{
		Raying_Core_Assert(!_instance, "Application alread exists!");
		_instance = this;

		_window = std::unique_ptr<Window>(Window::Create());
		_window->SetEventCallback(Bind_Event_Fn(OnEvent));

		_imguiLayer = new ImGuiLayer();
		PushLayer(_imguiLayer);

		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);


		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		_vbo.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ShaderAttribute::Color, ShaderDataType::Float4},
				{ShaderAttribute::Position, ShaderDataType::Float3}
			};
			_vbo->SetLayout(layout);
		}

		const BufferLayout& layout = _vbo->GetLayout();
		for (const BufferElement& element : layout)
		{
			glEnableVertexAttribArray(element.Index);
			glVertexAttribPointer(
				element.Index, element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type), 
				element.Normalized ? GL_TRUE : GL_FALSE, 
				layout.GetStride(), (const void*)element.Offset);
		}


		unsigned int indices[3] = {0, 1, 2};
		_ibo.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

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
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		_layerStack.PushOverlay(layer);
		layer->OnAttach();
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
			glClearColor(0, 0.3f, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(_vao);
			_shader->Bind();
			glDrawElements(GL_TRIANGLES, _ibo->GetCount(), GL_UNSIGNED_INT, nullptr);

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
