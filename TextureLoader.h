#pragma once

#include "Texture.h"

#include <unordered_map>
#include <string>
#include <memory>

namespace GfxEngine3D
{
	class TextureLoader
	{
	public:
		~TextureLoader();

		static TextureLoader& GetInstance();

		std::shared_ptr<Texture> LoadTexture(const std::string& name, const std::string& path);

		TextureLoader(const TextureLoader&) = delete;
		TextureLoader& operator=(const TextureLoader&) = delete;

	protected:
		TextureLoader();

	private:
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_textureCache;

		std::shared_ptr<Texture> GetTextureFromCache(const std::string& name);
	};
}
