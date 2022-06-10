#pragma once

#include <filesystem>

#include "Raying/Renderer/Texture.h"

namespace Raying {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path _currentDir;

		Ref<Texture2D> _dirIcon;
		Ref<Texture2D> _fileIcon;
	};
}
