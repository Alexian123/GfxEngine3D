#pragma once

class Texture
{
public:
	Texture(const char* path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

private:
	unsigned int m_id;
	int m_width, m_height;
	int m_channels;
};