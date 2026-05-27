#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <string>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

namespace GfxEngine3D
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
		~ShaderProgram();

		void Bind() const;
		void Unbind() const;

		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, int32_t value);
		void SetUniform(const std::string& name, uint32_t value);
		void SetUniform(const std::string& name, bool value);
		void SetUniform(const std::string& name, const glm::vec3& value);
		void SetUniform(const std::string& name, const glm::mat4& value);

		void SetDefaultVertexAttribute(int index, float x, float y) const;
		void SetDefaultVertexAttribute(int index, float x, float y, float z) const;
		void SetDefaultVertexAttribute(int index, float x, float y, float z, float w) const;

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram& other) = delete;
		ShaderProgram& operator=(const ShaderProgram& other) = delete;

	private:
		unsigned int m_id;
		std::unordered_map<std::string, int> m_uniformLocationCache;

		unsigned int CompileShader(unsigned int type, const char* source);
		int GetUniformLocation(const std::string& name);

		static std::string PreprocessShader(const std::string& path);
		static std::string ReadFile(const std::string& path);
		static std::string ProcessIncludes(
			const std::string& source,
			const std::filesystem::path& currentDir,
			std::unordered_set<std::string>& includedFiles
		);
	};
}

