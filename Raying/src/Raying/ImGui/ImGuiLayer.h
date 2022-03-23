#pragma once

#include "Raying/Layer.h"

namespace Raying
{
	class Raying_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& e);

	private:
		float _time = 0.0f;
	};
}


