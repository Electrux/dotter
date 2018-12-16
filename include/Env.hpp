/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef ENV_HPP
#define ENV_HPP

#include <string>

namespace Env
{
	const std::string GetVarVal( const std::string & var );
	bool CmdExistsInPath( const std::string & cmd );
	int Exec( const std::string & cmd );
	const std::string & ExpandPath( std::string & path );
	std::string ExpandPathConst( std::string path );
}

#endif // ENV_HPP