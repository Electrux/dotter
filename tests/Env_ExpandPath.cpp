/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <cstdlib>
#include "../include/Env.hpp"

int main()
{
	std::string home = Env::GetVarVal( "HOME" );
	if( home.empty() ) return 1;
	home += "/";

	assert( Env::ExpandPathConst( "${HOME}/" ) == home );
	return 0;
}