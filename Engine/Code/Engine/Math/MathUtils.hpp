#pragma once


//-----------------------------------------------------------------------------
class Vector2;
class PolarCoords;


//-----------------------------------------------------------------------------
const float PI = 3.14159265359f;
const float HALF_PI = PI / 2.f;


//-----------------------------------------------------------------------------
float Interpolate( float start, float end, float fractionTowardEnd );
float RangeMap( float inValue, float inStart, float inEnd, float outStart, float outEnd );
float CalcFractionInRange(float inValue, float rangeStart, float rangeEnd);
float ClampFloat( float inValue, float min, float max );
float ClampFloatZeroToOne( float inValue );


//-----------------------------------------------------------------------------
float DegreesToRadians( float degrees );
float RadiansToDegrees( float radians );
float CosDegrees( float degrees );
float SinDegrees( float degrees );
float Atan2Degrees( const Vector2& direction );


//-----------------------------------------------------------------------------
float CalcDistBetweenPoints( const Vector2& pos1, const Vector2& pos2 );
float CalcDistSquaredBetweenPoints( const Vector2& pos1, const Vector2& pos2 );
float CalcShortestAngularDisplacement( float fromDegrees, float toDegrees );
bool DoDiscsOverlap( const Vector2& center1, float radius1, const Vector2& center2, float radius2 );
bool IsPointInDisc( const Vector2& point, const Vector2& discCenter, float discRadius );


//-----------------------------------------------------------------------------
float GetRandomFloatZeroToOne();
float GetRandomFloatInRange( float min, float max );
int GetRandomIntInRange( float min, float max );
bool CheckRandomChance( float chanceForSuccess );