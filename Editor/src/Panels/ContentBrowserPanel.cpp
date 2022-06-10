#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

namespace Raying {

	static const std::filesystem::path _assetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: _currentDir(_assetPath)
	{
		_dirIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		_fileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (_currentDir != std::filesystem::path(_assetPath))
		{
			if (ImGui::Button("<-"))
			{
				_currentDir = _currentDir.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(_currentDir))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, _assetPath);
			std::string filenameString = relativePath.filename().string();

			Ref<Texture2D> icon = directoryEntry.is_directory() ? _dirIcon : _fileIcon;
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					_currentDir /= path.filename();
			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}

}
