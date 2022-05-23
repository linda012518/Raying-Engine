#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace Raying {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			Raying_Core_Assert(!HasComponent<T>(), "Entity already has Component!");
			return _scene->_registry.emplace<T>(_entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			Raying_Core_Assert(HasComponent<T>(), "Entity does not have Component!");
			return _scene->_registry.get<T>(_entityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return _scene->_registry.has<T>(_entityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			Raying_Core_Assert(HasComponent<T>(), "Entity does not have Component!");

			_scene->_registry.remove<T>(_entityHandle);
		}

		operator bool() const { return _entityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)_entityHandle; }

		bool operator == (const Entity& other) const
		{
			return _entityHandle == other._entityHandle && _scene == other._scene;
		}
		bool operator != (const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity _entityHandle{ entt::null };
		Scene* _scene = nullptr;
	};

}
