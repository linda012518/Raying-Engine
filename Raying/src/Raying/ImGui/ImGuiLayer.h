#pragma once

#include "Raying/Core/Layer.h"

#include "Raying/Events/ApplicationEvent.h"
#include "Raying/Events/KeyEvent.h"
#include "Raying/Events/MouseEvent.h"

namespace Raying
{
	class Raying_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		//virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void BlockEvents(bool block) { _blockEvents = block; }

		void SetDarkThemeColors();
	private:
		bool _blockEvents = true;
	};
}


