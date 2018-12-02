/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <cstdlib>

#include "../include/Config.hpp"

int main()
{
	Config conf;
	conf.LoadConfig( "sample_osx.yaml" );
	assert( conf.CheckEnvVars() == true );

	setenv( "SHELL", "/bin/bash", true );
	assert( conf.CheckEnvVars() == false );
	return 0;
}