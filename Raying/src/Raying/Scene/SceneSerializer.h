#pragma once

#include "Scene.h"

namespace Raying {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene> scene);

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool DeSerialize(const std::string& filepath);
		bool DeSerializeRuntime(const std::string& filepath);

	private:
		Ref<Scene> _scene;

	};

}
