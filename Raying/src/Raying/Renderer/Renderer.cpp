#include "hzpch.h"
#include "Renderer.h"
#include "VertexArray.h"

#include "Renderer2D.h"

namespace Raying {

	Scope<Renderer::SceneData> Renderer::_sceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		RendererCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RendererCommand::SetViewport(0, 0, width, height);
	}


	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		_sceneData->ViewProjectionMatrix = camera.GetVPMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vetrexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("_ViewProjection", _sceneData->ViewProjectionMatrix);
		shader->SetMat4("_Transform", transform);

		vetrexArray->Bind();
		RendererCommand::DrawIndexed(vetrexArray);
	}

}