#pragma once


#include <string>

#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/Vector2.hpp"


//-----------------------------------------------------------------------------
class Texture;
class AABB2;


//-----------------------------------------------------------------------------
class SpriteSheet
{
public:
	SpriteSheet( const std::string& imageFilePath, int tilesWide, int tilesHigh, int tileWidth, int tileHeight );

	AABB2 GetTexCoordsFromSpriteCoords( int spriteX, int spriteY ) const; // mostly for atlases
	AABB2 GetTexCoordsFromSpriteIndex( int spriteIndex ) const; // mostly for sprite animations, ensure 0-based index
	int GetNumSprites() const;
	Texture* GetAtlasTexture() const { return m_spriteSheetTexture; }


private:
	Texture* 	m_spriteSheetTexture;
	IntVector2	m_spriteLayout;	// # of sprites total (across and down) on the sheet
	Vector2	m_texelsPerSprite; // One step into a tile, kept as a fraction 1/tileSize.
	IntVector2	m_tileSize;
};
