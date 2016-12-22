#include "Engine/Math/AABB2.hpp"


//--------------------------------------------------------------------------------------------------------------
AABB2::AABB2( const Vector2& mins, const Vector2& maxs )
	: mins(mins)
	, maxs(maxs)
{
}


//--------------------------------------------------------------------------------------------------------------
AABB2::AABB2( float minX, float minY, float maxX, float maxY )
	: mins( Vector2( minX, minY ) )
	, maxs( Vector2( maxX, maxY ) )
{
}


//--------------------------------------------------------------------------------------------------------------
AABB2::AABB2( int minX, int minY, int maxX, int maxY )
	: mins( Vector2( static_cast<float>( minX ), static_cast<float>( minY ) ) )
	, maxs( Vector2( static_cast<float>( maxX ), static_cast<float>( maxY ) ) )
{
}