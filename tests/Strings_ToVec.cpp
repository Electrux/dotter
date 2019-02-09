/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include "../include/Strings.hpp"

int main()
{
	const char * argv[] = { "Hello", "World", "You", "Are", "Big" };
	int argc = 5;
	auto args = Str::ToVec( argc, ( const char ** )argv );
	assert( args.size() == 5 );
	assert( args[ 2 ] == "You" );
	return 0;
}
