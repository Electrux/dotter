/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.

*/

#include <iostream>

#include "../include/Strings.hpp"
#include "../include/Core.hpp"
#include "../include/Dotter/Handler.hpp"

int main( int argc, char ** argv )
{
	auto args = Str::ToVec( argc, ( const char ** )argv );
	if( !Core::Init() ) return -1;

	return Dot::Handle( args );
}