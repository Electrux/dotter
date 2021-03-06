/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef INFO_HPP
#define INFO_HPP

#include <string>

const std::string & SelfConfigFile();
const std::string & WorkspaceLoc();
std::string ConfigFile( const bool use_platform = false );
const std::string & Platform();

#endif // INFO_HPP