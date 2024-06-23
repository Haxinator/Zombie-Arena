#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::m_s_Instance = nullptr;

TextureHolder::TextureHolder()
{
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}

Texture& TextureHolder::GetTexture(string const& filename)
{
	//Get a reference to m_Texture using m_s_Instance
	//type map<string, Texture>
	auto& m = m_s_Instance->m_Textures;

	//create iterator to hold key value pair
	//search for required pair using filename
	//type map<string, Texture>::iterator
	auto keyValuePair = m.find(filename);

	//Did we find a match?
	if (keyValuePair != m.end())
	{
		//return the texture
		return keyValuePair->second;
	}
	else {
		//file name not found create new pair
		auto& texture = m[filename];
		texture.loadFromFile(filename);

		return texture;
	}
}