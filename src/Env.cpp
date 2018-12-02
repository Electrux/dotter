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

#include "../include/Strings.hpp"
#include "../include/FileIO.hpp"
#include "../include/Env.hpp"

const std::string Env::GetVarVal( const std::string & var )
{
	auto val = std::getenv( var.c_str() );
	if( val == nullptr ) return "";
	return val;
}

bool Env::CmdExistsInPath( const std::string & cmd )
{
	std::string path_tmp = GetVarVal( "PATH" );
	if( path_tmp.empty() ) return false;

	auto paths = Str::Delimit( path_tmp, ':' );
	for( auto & p : paths ) {
		if( FS::LocExists( p + "/" + cmd ) ) return true;
	}
	return false;
}

int Env::Exec( const std::string & cmd )
{
	return std::system( cmd.c_str() );
}