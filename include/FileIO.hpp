/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef FILE_IO_HPP
#define FILE_IO_HPP

#include <string>
#include <filesystem>

#include "Env.hpp"

namespace fs = std::__fs::filesystem;

namespace FS
{
	inline bool LocExists( const std::string & loc )
	{
		return fs::exists( loc );
	}

	inline bool CreateDir( const std::string & loc )
	{
		return !Env::Exec( "mkdir -p " + loc );
	}

	inline bool CreateDirSudo( const std::string & loc )
	{
		return !Env::Exec( "sudo mkdir -p " + loc );
	}
}

#endif // FILE_IO_HPP