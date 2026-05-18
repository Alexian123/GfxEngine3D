#pragma once

class Texture
{
public:
	Texture(const char* path);
	~Texture();

	unsigned int GetID() const { return m_id; }
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	int GetChannels() const { return m_channels; }

	void Bind(int slot) const;
	void Unbind() const;

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

private:
	unsigned int m_id;
	int m_width, m_height;
	int m_channels;
};