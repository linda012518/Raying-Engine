#include <Raying.h>
#include <Raying/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Raying {

	class RayingEditor : public Application
	{
	public:
		RayingEditor(ApplicationCommandLineArgs args)
			: Application("Raying Editor", args)
		{
			PushLayer(new EditorLayer());
		}

		~RayingEditor()
		{

		}

	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new RayingEditor(args);
	}
}
