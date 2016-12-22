#pragma once


#include <map>

#include "Engine/Math/IntVector2.hpp"


//-----------------------------------------------------------------------------
class Texture
{
public:
	static Texture* CreateOrGetTexture( const std::string& imageFilePath );

	static Texture* GetTextureByName( const std::string& imageFilePath );
	unsigned int GetTextureID() const { return m_openglTextureID; }
	IntVector2 GetTextureDimensions() const { return m_texelSize; }


private:
	Texture( const std::string& imageFilePath );

	static std::map<std::string, Texture*> s_textureRegistry;
	unsigned int m_openglTextureID;
	IntVector2 m_texelSize;
};