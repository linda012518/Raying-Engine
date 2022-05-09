#include <Raying.h>
#include <Raying/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Raying {

	class RayingEditor : public Application
	{
	public:
		RayingEditor()
			: Application("Raying Editor")
		{
			PushLayer(new EditorLayer());
		}

		~RayingEditor()
		{

		}

	};

	Application* CreateApplication()
	{
		return new RayingEditor();
	}
}