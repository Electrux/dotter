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
	std::string m_dir_repo;

	std::unordered_map< std::string, std::vector< std::string > > m_env_vars;

	std::vector< std::string > m_dirs;
	std::vector< std::string > m_dirs_sudo;

	std::vector< lnk_block > m_lnks;
	std::vector< lnk_block > m_lnks_sudo;

	std::vector< std::string > m_final_cmds;
public:
	bool LoadConfig( const std::string & file_name, const std::string & dir_repo = "" );

	const std::unordered_map< std::string, std::vector< std::string > > &
	GetEnvVars();

	bool CheckEnvVars();

	bool ValidateDirectories();

	bool LnksExist();

	bool InstallLnks();

	bool PerfPostLnkCmds();
};

#endif // CONFIG_HPP
