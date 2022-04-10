#include "hzpch.h"
#include "Renderer.h"
#include "VertexArray.h"

namespace Raying {

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vetrexArray)
	{
		vetrexArray->Bind();
		RendererCommand::DrawIndexed(vetrexArray);
	}

}