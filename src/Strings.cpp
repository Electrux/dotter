/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <string>
#include <vector>

#include "../include/Strings.hpp"

std::vector< std::string > Str::Delimit( const std::string & str, const char delim )
{
	std::vector< std::string > vec;
	std::string tmp;
	for( auto c : str ) {
		if( c == delim ) {
			vec.push_back( tmp );
			tmp.clear();
			continue;
		}
		tmp += c;
	}
	if( !tmp.empty() ) {
		vec.push_back( tmp );
	}
	return vec;
}

int Str::IsOneOf( const std::vector< std::string > & vec, const std::string & str )
{
	for( size_t i = 0; i < vec.size(); ++i ) {
		if( str == vec[ i ] ) return i;
	}
	return -1;
}

std::vector< std::string > Str::ToVec( const int argc, const char ** argv )
{
	std::vector< std::string > vec;
	for( int i = 0; i < argc; ++i ) {
		vec.push_back( argv[ i ] );
	}
	return vec;
}