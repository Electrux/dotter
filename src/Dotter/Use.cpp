/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../../include/Info.hpp"
#include "../../include/Core.hpp"
#include "../../include/Env.hpp"
#include "../../include/FileIO.hpp"
#include "../../include/Config.hpp"
#include "../../include/Dotter/Use.hpp"

static int InstallRepo( const std::vector< std::string > & args, const std::string & dir_repo, const std::string & config_file );

int Dot::Use( const std::vector< std::string > & args )
{
	if( args.size() < 3 ) {
		std::cout << "Usage: " << args[ 0 ] << " use <Username/Repo>\n";
		return -2;
	}

	std::string dir_repo = args[ 2 ];
	std::replace( dir_repo.begin(), dir_repo.end(), '/', '_' );
	dir_repo = WorkspaceLoc() + "/" + dir_repo;

	if( !FS::LocExists( dir_repo ) ) {
		std::cerr << "Error: The repository doesn't exist! Use the 'use' argument to download repositories!\n";
		return -3;
	}

	std::string config_file = dir_repo + "/" + ConfigFile( true );
	bool is_generic_conf = false;
	std::cout << "Info: Finding OS specific config file ... ";
	if( !FS::LocExists( config_file ) ) {
		std::cout << "Failed ... Finding generic config ... ";
		config_file = dir_repo + "/" + ConfigFile();
		is_generic_conf = true;
		if( !FS::LocExists( config_file ) ) {
			std::cout << "Failed\n";
			std::cerr << "Error: No configuration file found! Cannot continue!\n";
			return 1;
		}
	}
	std::cout << "Success\n";

	return InstallRepo( args, dir_repo, config_file );
}

static int InstallRepo( const std::vector< std::string > & args, const std::string & dir_repo, const std::string & config_file )
{
	Config conf;
	if( !conf.LoadConfig( config_file, dir_repo ) ) {
		std::cerr << "Error: Failed loading the config file: " << config_file << "! Cannot continue!\n";
		return -3;
	}

	if( !conf.CheckEnvVars() ) {
		std::cerr << "Error: Unmet required environment variables! Cannot continue!\n";
		return -3;
	}

	if( !conf.ValidateDirectories() ) {
		std::cerr << "Error: Unmet required directories! Cannot continue!\n";
		return -3;
	}

	if( !conf.LnksExist() ) {
		std::cerr << "Error: Required source links do not exist! Cannot continue!\n";
		return -3;
	}

	if( !conf.InstallLnks() ) {
		std::cerr << "Error: Unable to install links! Cannot continue!\n";
		return -3;
	}
	if( !conf.PerfPostLnkCmds() ) {
		std::cerr << "Error: Failed to perform post link commands! Cannot continue!\n";
		return -3;
	}

	return 0;
}