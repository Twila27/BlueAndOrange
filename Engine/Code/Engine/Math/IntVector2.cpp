#include "Engine/Math/IntVector2.hpp"


//--------------------------------------------------------------------------------------------------------------
// Do-nothing default ctor: because it saves time to leave trash values rather than allocate and initialize.
IntVector2::IntVector2()
	: x()
	, y()
{
}


//--------------------------------------------------------------------------------------------------------------
IntVector2::IntVector2( int initialX, int initialY )
	: x( initialX )
	, y( initialY )
{
}


//--------------------------------------------------------------------------------------------------------------
IntVector2::IntVector2( const IntVector2& vectorToCopy )
	: x( vectorToCopy.x )
	, y( vectorToCopy.y )
{
}


//--------------------------------------------------------------------------------------------------------------
const IntVector2 IntVector2::operator*( float scalarToScaleBy ) const
{
	return IntVector2( x * (int)scalarToScaleBy, y * (int)scalarToScaleBy );
}


//--------------------------------------------------------------------------------------------------------------
bool IntVector2::operator==( const IntVector2& compareTo ) const
{
	return ( x == compareTo.x ) & ( y == compareTo.y );
}
