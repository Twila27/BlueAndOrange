#include "Engine/Math/Vector2.hpp"


#include <math.h>

#include "Engine/Error/ErrorWarningAssert.hpp"


//--------------------------------------------------------------------------------------------------------------
// Do-nothing default ctor: because it saves time to leave trash values rather than allocate and initialize.
Vector2::Vector2() 
	: x()
	, y()
{
}


//--------------------------------------------------------------------------------------------------------------
Vector2::Vector2( float initialX, float initialY )
: x( initialX )
, y( initialY )
{
}	


//--------------------------------------------------------------------------------------------------------------
Vector2::Vector2( const Vector2& vectorToCopy ) 
: x( vectorToCopy.x )
, y( vectorToCopy.y )
{
}


//--------------------------------------------------------------------------------------------------------------
Vector2 Vector2::operator+( const Vector2& vectorToAdd ) const
{
	return Vector2( x + vectorToAdd.x, y + vectorToAdd.y );
}


//--------------------------------------------------------------------------------------------------------------
Vector2 Vector2::operator-( const Vector2& vectorToAdd ) const
{
	return Vector2( x - vectorToAdd.x, y - vectorToAdd.y );
}


//--------------------------------------------------------------------------------------------------------------
Vector2 Vector2::operator-()
{
	x = -x;
	y = -y;
	return Vector2( x, y );
}

//--------------------------------------------------------------------------------------------------------------
Vector2 Vector2::operator*( float scalarToScaleBy ) const
{
	return Vector2( x * scalarToScaleBy, y * scalarToScaleBy );
}


//--------------------------------------------------------------------------------------------------------------
Vector2 Vector2::operator/( float scalarToScaleBy ) const
{
	if (scalarToScaleBy == 0.f) ERROR_AND_DIE( "Divide By Scalar Zero" );
	return Vector2( x / scalarToScaleBy, y / scalarToScaleBy );
}


//--------------------------------------------------------------------------------------------------------------
bool Vector2::operator==( const Vector2& compareTo ) const
{
	return ( x == compareTo.x ) && ( y == compareTo.y );
}

//--------------------------------------------------------------------------------------------------------------
void Vector2::operator+=( const Vector2& vectorToAdd )
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
}


//--------------------------------------------------------------------------------------------------------------
void Vector2::operator-=( const Vector2& vectorToSubtract )
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
}


//--------------------------------------------------------------------------------------------------------------
void Vector2::operator*=( const float scalarToScaleBy )
{
	x *= scalarToScaleBy;
	y *= scalarToScaleBy;
}


//--------------------------------------------------------------------------------------------------------------
void Vector2::operator=( const Vector2& vectorToCopy )
{
	x = vectorToCopy.x;
	y = vectorToCopy.y;
}


//--------------------------------------------------------------------------------------------------------------
float Vector2::CalcLength( ) const
{
	return sqrt( ( x * x ) + ( y * y ) );
}


//--------------------------------------------------------------------------------------------------------------
void Vector2::Normalize( )
{
	float len = CalcLength();
	if ( len == 0.f ) ERROR_AND_DIE( "Normalizing By Length Zero" );

	x /= len;
	y /= len;
}


//--------------------------------------------------------------------------------------------------------------
void Vector2::SetXY(float newX, float newY) 
{
	x = newX;
	y = newY;
}