#include "Engine/Renderer/Rgba.hpp"


//--------------------------------------------------------------------------------------------------------------
Rgba::Rgba()
	: red( 1.f )
	, green( 1.f )
	, blue( 1.f )
	, alphaOpacity( 1.f )
{
}


//--------------------------------------------------------------------------------------------------------------
Rgba::Rgba( float red, float green, float blue, float alphaOpacity /*=1.f*/ )
	: red( red )
	, blue( blue )
	, green( green )
	, alphaOpacity( alphaOpacity )
{
}

bool Rgba::operator==( const Rgba& compareTo ) const
{
	return ( red == compareTo.red ) && ( green == compareTo.green ) && ( blue == compareTo.blue );
}
