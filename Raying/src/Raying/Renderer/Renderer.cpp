#include "hzpch.h"
#include "Renderer.h"
#include "VertexArray.h"

namespace Raying {

	Renderer::SceneData* Renderer::_sceneData = new Renderer::SceneData();

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		_sceneData->ViewProjectionMatrix = camera.GetVPMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vetrexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("_ViewProjection", _sceneData->ViewProjectionMatrix);
		shader->UploadUniformMat4("_Transform", transform);

		vetrexArray->Bind();
		RendererCommand::DrawIndexed(vetrexArray);
	}

}