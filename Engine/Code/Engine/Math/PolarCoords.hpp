#pragma once


class PolarCoords
{
public:
	float radius;
	float thetaRadians;

	PolarCoords();
	PolarCoords( float initialX, float initialY );
	PolarCoords( const PolarCoords& coordsToCopy );
};