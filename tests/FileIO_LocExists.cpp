/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include "../include/FileIO.hpp"

int main()
{
	assert( FS::LocExists( "src/" ) );
	assert( FS::LocExists( "src/main.cpp" ) );
	return 0;
}