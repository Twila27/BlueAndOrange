#pragma once


#include "Engine/Math/Vector2.hpp"


class AABB2
{
public:
	Vector2 mins;
	Vector2 maxs;

	AABB2( const Vector2& mins, const Vector2& maxs );
	AABB2( float minX, float minY, float maxX, float maxY );
	AABB2( int minX, int minY, int maxX, int maxY );
};