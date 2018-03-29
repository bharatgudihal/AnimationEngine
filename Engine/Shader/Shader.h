#pragma once

#include <Externals/glm/Includes.h>
#include <Engine/Utility/ReferenceCounted.h>

namespace Engine {
	namespace Graphics {
		class Shader {
		public:

			static Shader* CreateShader(const char* vertexShaderPath, const char* fragmentShaderPath);
			static void DestroyShader(Shader* shader);

			void Use();
			void SetBool(const char* name, bool value) const;
			void SetInt(const char* name, int value) const;
			void SetFloat(const char* name, float value) const;
			void SetVector(const char* name, const glm::vec3& vector);
			void SetMatrix(const char* name, const glm::mat4& matrix) const;
			REFERENCE_COUNT_FUNCTIONS

		private:
			Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
			~Shader();
			Shader(Shader& other);
			Shader& operator=(Shader& other);
			unsigned int shaderId;
			REFERENCE_COUNT_VARIABLES
		};
	}
}