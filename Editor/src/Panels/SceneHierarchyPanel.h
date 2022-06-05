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

		Entity GetSelectedEntity() const { return _selectionContext; }
		void SetSelectedEntity(Entity entity) { _selectionContext = entity; }
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> _context;
		Entity _selectionContext;

	};
}
