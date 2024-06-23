#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <SFML/Graphics.hpp>
#include <map>

using namespace sf;
using namespace std;

class TextureHolder
{
private:
	//A map that holds string and textures
	map<string, Texture> m_Textures;

	//pointer to instance of textureholder
	static TextureHolder* m_s_Instance;

public:
	TextureHolder();
	static Texture& GetTexture(string const& filename);
};

#endif