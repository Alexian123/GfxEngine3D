#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

TextureLoader::~TextureLoader()
{
	m_textureCache.clear();
}

TextureLoader& TextureLoader::GetInstance()
{
	static TextureLoader instance;
	return instance;
}

std::shared_ptr<Texture> TextureLoader::LoadTexture(const std::string& name, const std::string& path)
{
	if (name.empty() || path.empty())
		return nullptr;
	auto cachedTexture = GetTextureFromCache(name);
	if (cachedTexture)
		return cachedTexture;

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (!data) {
		std::cerr << "ERROR::TEXTURE::FAILED_TO_LOAD\n" << stbi_failure_reason() << std::endl;
		stbi_image_free(data);
		return nullptr;
	}
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(
		width, height, channels,
		std::vector<unsigned char>(data, data + width * height * channels)
	);
	stbi_image_free(data);

	m_textureCache[name] = texture;
	return texture;
}

TextureLoader::TextureLoader()
{
	m_textureCache.reserve(10);	// reserve some space for caching textures
}

std::shared_ptr<Texture> TextureLoader::GetTextureFromCache(const std::string& name)
{
	auto it = m_textureCache.find(name);
	if (it != m_textureCache.end())
		return it->second;
	else
		return nullptr;
}