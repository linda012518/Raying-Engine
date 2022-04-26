#include "hzpch.h"
#include "RendererCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Raying {

	Scope<RendererAPI> RendererCommand::_rendererAPI = RendererAPI::Create();

}