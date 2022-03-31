#pragma once

#include "Raying/Layer.h"

#include "Raying/Events/ApplicationEvent.h"
#include "Raying/Events/KeyEvent.h"
#include "Raying/Events/MouseEvent.h"

namespace Raying
{
	class Raying_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float _time = 0.0f;
	};
}


