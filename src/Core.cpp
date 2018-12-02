/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <yaml-cpp/yaml.h>

#include "../include/Info.hpp"
#include "../include/Env.hpp"
#include "../include/FileIO.hpp"
#include "../include/Core.hpp"

bool Core::Init()
{
	if( !FS::LocExists( WorkspaceLoc() ) && Env::Exec( "mkdir -p " + WorkspaceLoc() ) != 0 ) {
		std::cerr << "mkdir failed for workspace location: " << WorkspaceLoc() << "\n";
		return false;
	}

	if( !FS::LocExists( SelfConfigFile() ) ) {
		YAML::Node cfg;
		cfg[ "repo" ] = "none";
		cfg[ "stage" ] = "-1";

		std::ofstream cfgfile( SelfConfigFile() );
		cfgfile << cfg;
	}

	return true;
}