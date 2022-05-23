#pragma once

#include "Raying/Core/Base.h"
#include "Raying/Scene/Scene.h"
#include "Raying/Core/Log.h"
#include "Raying/Scene/Entity.h"

namespace Raying {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> _context;
		Entity _selectionContext;

	};
}
