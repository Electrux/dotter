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
	assert( Str::IsOneOf( { "One", "Two", "Three" }, "Two" ) == 1 );
	assert( Str::IsOneOf( { "One", "Two", "Three" }, "Zero" ) == -1 );
	return 0;
}