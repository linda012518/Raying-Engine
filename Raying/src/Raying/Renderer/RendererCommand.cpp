#include "hzpch.h"
#include "RendererCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Raying {

	RendererAPI* RendererCommand::_rendererAPI = new OpenGLRendererAPI();

}