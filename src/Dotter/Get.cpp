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
#include "../../include/Dotter/Get.hpp"

int Dot::Get( const std::vector< std::string > & args )
{
	if( args.size() < 3 ) {
		std::cout << "Usage: " << args[ 0 ] << " get <Username/Repo>\n";
		return -2;
	}

	// check if git exists in PATH
	if( !Env::CmdExistsInPath( "git" ) ) {
		std::cout << "Error: git not installed! please install it first!\n";
		return -3;
	}

	std::string dir_repo = args[ 2 ];
	std::replace( dir_repo.begin(), dir_repo.end(), '/', '_' );

	std::string cmd;
	bool clone = true;
	if( FS::LocExists( WorkspaceLoc() + "/" + dir_repo ) ) {
		std::cerr << "Info: Already downloaded this repository, will pull for latest changes\n";
		cmd = "git -C " + WorkspaceLoc() + "/" + dir_repo + " pull";
		clone = false;
	} else {
		std::cout << "Info: Cloning the repository " << args[ 2 ] << " in " << WorkspaceLoc() << "/" << dir_repo << "\n";
		cmd = "git clone https://github.com/" + args[ 2 ] + ".git " + WorkspaceLoc() + "/" + dir_repo;
	}

	int res = Env::Exec( cmd );
	if( res != 0 ) {
		if( clone ) {
			std::cerr << "Failed cloning the given repository. Either the repository doesn't exist, or the internet isn't working!"
				<< " Please check the git output!\n";
		} else {
			std::cerr << "Failed pulling the given repository. Perhaps the internet isn't working!"
				<< " Please check the git output!\n";
		}
		return res;
	}

	return 0;
}