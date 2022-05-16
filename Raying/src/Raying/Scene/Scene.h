#pragma once

#include "entt.hpp"

#include "Raying/Core/Timestep.h"

namespace Raying {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		entt::registry& Reg() { return _registry; }

		void OnUpdate(Timestep ts);

	private:
		entt::registry _registry;

		friend class Entity;
	};

}