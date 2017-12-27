#pragma once

#include <Externals/glm/Includes.h>

namespace Engine {
	namespace Graphics {
		class Shader {
		public:

			Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
			~Shader();

			void Use();

			void SetBool(const char* name, bool value) const;
			void SetInt(const char* name, int value) const;
			void SetFloat(const char* name, float value) const;
			void SetVector(const char* name, const glm::vec3& vector);
			void SetMatrix(const char* name, const glm::mat4& matrix) const;			

		private:
			unsigned int shaderId;

		};
	}
}