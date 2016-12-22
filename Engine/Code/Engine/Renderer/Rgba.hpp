#pragma once


struct Rgba
{
	float red;
	float green;
	float blue;
	float alphaOpacity;

	Rgba();
	Rgba( float red, float green, float blue, float alphaOpacity = 1.f );

	bool operator==( const Rgba& compareTo ) const; //Currently NOT comparing alpha.
};