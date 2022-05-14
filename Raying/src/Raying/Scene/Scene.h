#pragma once

#include "entt.hpp"

#include "Raying/Core/Timestep.h"

namespace Raying {

	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		entt::registry& Reg() { return _registry; }

		void OnUpdate(Timestep ts);

	private:
		entt::registry _registry;
	};

}