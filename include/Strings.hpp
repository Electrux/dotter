/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <string>
#include <vector>

namespace Str
{
	std::vector< std::string > Delimit( const std::string & str, const char delim );
	int IsOneOf( const std::vector< std::string > & vec, const std::string & str );
	std::vector< std::string > ToVec( const int argc, const char ** argv );
}

#endif // STRINGS_HPP