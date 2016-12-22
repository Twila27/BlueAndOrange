// By Squirrel Eiserloh

#pragma once
//-----------------------------------------------------------------------------------------------
#include <string>


//-----------------------------------------------------------------------------------------------
const std::string Stringf( const char* format, ... ); //Use this most often.
const std::string Stringf( const int maxLength, const char* format, ... ); //For 1000s+ chars.