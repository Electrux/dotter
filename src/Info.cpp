/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "../include/Info.hpp"

static std::string GetHomeDir()
{
	static const char * homedir = nullptr;
	if( homedir != nullptr ) return homedir;

	if( ( homedir = getenv( "HOME" ) ) == nullptr ) {
		homedir = getpwuid( getuid() )->pw_dir;
	}
	return homedir;
}

const std::string & SelfConfigFile()
{
	static const std::string loc = GetHomeDir() + "/.dottcfg.yaml";
	return loc;
}

const std::string & WorkspaceLoc()
{
	static const std::string loc = GetHomeDir() + "/.dott";
	return loc;
}

std::string ConfigFile( const bool use_platform )
{
	std::string conf_file = "dott";
	if( use_platform ) conf_file +=  "_" + Platform();
	conf_file += ".yaml";
	return conf_file;
}

const std::string & Platform()
{
	static const std::string platform = 
#ifdef __linux__
		"linux"
#elif __APPLE__
		"osx"
#else
		"other"
#endif
	;
	return platform;
}