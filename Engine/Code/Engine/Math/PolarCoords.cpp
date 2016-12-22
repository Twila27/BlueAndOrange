#include "Engine/Math/PolarCoords.hpp"


//--------------------------------------------------------------------------------------------------------------
// Do-nothing ctor: because it's faster to leave trash values than to allocate and initialize.
PolarCoords::PolarCoords()
	: radius()
	, thetaRadians()
{
}


//--------------------------------------------------------------------------------------------------------------
PolarCoords::PolarCoords( float initialRadius, float initialThetaRadians )
	: radius( initialRadius )
	, thetaRadians( initialThetaRadians )
{
}


//--------------------------------------------------------------------------------------------------------------
PolarCoords::PolarCoords( const PolarCoords& coordsToCopy )
	: radius( coordsToCopy.radius )
	, thetaRadians( coordsToCopy.thetaRadians )
{
}
