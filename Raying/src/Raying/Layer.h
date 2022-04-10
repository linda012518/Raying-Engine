#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Raying
{
	class Raying_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}
		
		inline const std::string& GetName() const { return _debugName; }

	protected:
		std::string _debugName;
	};
}


