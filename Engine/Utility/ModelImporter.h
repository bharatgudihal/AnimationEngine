#pragma once

#include <string>

namespace Engine {

	class Actor;

	namespace Utility {
		void ImportModel(const std::string path, Actor*& o_actor);
	}
}