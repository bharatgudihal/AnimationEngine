#pragma once

#include <string>
#include <Externals/glm/Includes.h>

namespace Engine {
	namespace Graphics {
		class Shader {
		public:

			Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
			~Shader();

			void Use();

			void SetBool(const std::string& name, bool value) const;
			void SetInt(const std::string& name, int value) const;
			void SetFloat(const std::string& name, float value) const;
			void SetVector(const std::string& name, const glm::vec3& vector);
			void SetMatrix(const std::string& name, const glm::mat4& matrix) const;

		private:
			unsigned int shaderId;

		};
	}
}