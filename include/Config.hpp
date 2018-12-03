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

struct dir
{
	std::string dir;
	bool sudo;
};

struct lnk
{
	std::string src;
	std::string dest;
};

struct lnk_block
{
	std::string prefix_src;
	std::string prefix_dest;
	std::vector< lnk > lnks;
};

class Config
{
	std::unordered_map< std::string, std::vector< std::string > > m_env_vars;
	std::vector< dir > m_dirs;
	std::vector< lnk_block > m_lnks;
	std::vector< std::string > m_final_cmds;
public:
	bool LoadConfig( const std::string & file_name );

	const std::unordered_map< std::string, std::vector< std::string > > &
	GetEnvVars();

	bool CheckEnvVars();
};

#endif // CONFIG_HPP
