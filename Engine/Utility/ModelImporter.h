#pragma once

namespace Engine {

	namespace Graphics {
		class Mesh;
	}

	namespace Utility {
		Graphics::Mesh* ImportModel(const char* path);
	}
}