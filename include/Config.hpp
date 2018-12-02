/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <unordered_map>

class Config
{
	std::unordered_map< std::string, std::vector< std::string > > m_env_vars;
public:
	bool LoadConfig( const std::string & file_name );

	const std::unordered_map< std::string, std::vector< std::string > > &
	GetEnvVars();

	bool CheckEnvVars();
};

#endif // CONFIG_HPP
