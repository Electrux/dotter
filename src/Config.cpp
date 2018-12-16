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
#include <unordered_map>
#include <yaml-cpp/yaml.h>

#include "../include/Strings.hpp"
#include "../include/FileIO.hpp"
#include "../include/Env.hpp"
#include "../include/Config.hpp"

static void FetchDirs( YAML::Node & conf, const std::string & conf_key, std::vector< std::string > & m_dirs );
static void FetchLnks( YAML::Node & conf, const std::string & conf_key,
	std::vector< lnk_block > & m_lnks, const std::string & repo_dir );

bool Config::LoadConfig( const std::string & file_name, const std::string & dir_repo )
{
	if( !dir_repo.empty() ) m_dir_repo = dir_repo;

	if( !FS::LocExists( file_name ) ) {
		std::cerr << "Unable to open configuration file: " << file_name << "\n";
		return false;
	}

	YAML::Node conf = YAML::LoadFile( file_name );

	std::cout << "Info: Loading configuration file ...\n";
	// First, retrieve Env
	if( conf[ "env" ] ) {
		for( auto v : conf[ "env" ] ) {
			std::string key;
			std::vector< std::string > val;
			if( v.IsMap() ) {
				auto m = v.begin();
				key = m->first.as< std::string >();
				val = Str::Delimit( m->second.as< std::string >(), ';' );
			} else {
				key = v.as< std::string >();
			}
			m_env_vars[ key ] = val;
		}
	}
	std::cout << "  => " << m_env_vars.size() << " env vars\n";

	// Now, fetch the directories and sudo directories to be created
	FetchDirs( conf, "dirs", m_dirs );
	FetchDirs( conf, "dirs_sudo", m_dirs_sudo );

	// Now, retrieve the links and sudo links
	FetchLnks( conf, "lnk", m_lnks, m_dir_repo );
	FetchLnks( conf, "lnk_sudo", m_lnks_sudo, m_dir_repo );

#ifdef __DEBUG__
	std::cout << "Links:\n";
	for( auto & lnk_blk : m_lnks ) {
		std::cout << "=> Block: " << lnk_blk.prefix_src << ", " << lnk_blk.prefix_dest << "\n";
		for( auto & lnk : lnk_blk.lnks ) {
			std::cout << "===> " << lnk.src << " -> " << lnk.dest << "\n";
		}
	}
#endif

	// Fetch the post link commands
	if( conf[ "post_lnk_cmds" ] ) {
		for( auto lnk : conf[ "post_lnk_cmds" ] ) {
			m_final_cmds.push_back( lnk.as< std::string >() );
		}
	}
	std::cout << "  => " << m_final_cmds.size() << " post link commands\n";

	return true;
}

const std::unordered_map< std::string, std::vector< std::string > > &
Config::GetEnvVars()
{
	return m_env_vars;
}

bool Config::CheckEnvVars()
{
	if( m_env_vars.empty() ) return true;
	std::cout << "Info: Checking if required env vars exist ...\n";
	for( auto & v : m_env_vars ) {
		std::cout << "  => '" << v.first << "' ... ";
		std::string val = Env::GetVarVal( v.first );

		if( val.empty() ) {
			std::cout << "No\n";
			return false;
		}

		std::cout << "Yes";
		if( !v.second.empty() ) {
			int pos = Str::IsOneOf( v.second, val );
			if( pos < 0 ) {
				std::cout << " (Invalid value: " << val << ")\n";
				return false;
			}
			std::cout << " (" << v.second[ pos ] << ")";
		}
		std::cout << "\n";
	}
	return true;
}

bool Config::ValidateDirectories()
{
	if( m_dirs.empty() && m_dirs_sudo.empty() ) return true;
	if( !m_dirs.empty() ) std::cout << "Info: Checking if required directories exist ...\n";
	for( auto & dir : m_dirs ) {
		std::cout << "  => '" << dir << "' ... ";
		if( FS::LocExists( dir ) ) {
			std::cout << "Yes\n";
			continue;
		}
		std::cout << "No ... Creating ... ";
		if( FS::CreateDir( dir ) ) {
			std::cout << "Done\n";
		} else {
			std::cout << "Failed\n";
			std::cout << "Could not create a required directory! Cannot continue!\n";
			return false;
		}
	}

	if( !m_dirs_sudo.empty() ) std::cout << "Info: Checking if required sudo directories exist ...\n";
	for( auto & dir : m_dirs_sudo ) {
		std::cout << "  => '" << dir << "' ... ";
		if( FS::LocExists( dir ) ) {
			std::cout << "Yes\n";
			continue;
		}
		std::cout << "No ... Creating ...\n";
		std::cout << "Info: Enter sudo details if asked!\n";
		if( FS::CreateDirSudo( dir ) ) {
			std::cout << "=> Directory creation successful! Continuing!\n";
		} else {
			std::cout << "=> Could not create a required sudo directory! Cannot continue!\n";
			return false;
		}
	}
	return true;
}

bool Config::LnksExist()
{
	if( m_lnks.empty() && m_lnks_sudo.empty() ) return true;

	int ctr = 1;
	for( auto & lb : m_lnks ) {
		std::cout << "Info: Checking if link[" << ctr << "] locations exist ...\n";
		for( auto & l : lb.lnks ) {
			auto src_loc = lb.prefix_src;
			if( * ( src_loc.end() - 1 ) != '/' ) src_loc += "/";
			src_loc += l.src;
			std::cout << "  => '" << src_loc << "' ... ";
			if( FS::LocExists( src_loc ) ) {
				std::cout << "Yes\n";
			} else {
				std::cout << "No\n";
				return false;
			}
		}
		++ctr;
	}

	ctr = 1;
	for( auto & lb : m_lnks_sudo ) {
		std::cout << "Info: Checking if sudo link[" << ctr << "] locations exist ...\n";
		for( auto & l : lb.lnks ) {
			auto src_loc = lb.prefix_src;
			if( * ( src_loc.end() - 1 ) != '/' ) src_loc += "/";
			src_loc += l.src;
			std::cout << "  => '" << src_loc << "' ... ";
			if( FS::LocExists( src_loc ) ) {
				std::cout << "Yes\n";
			} else {
				std::cout << "No\n";
				return false;
			}
		}
		++ctr;
	}

	return true;
}

bool Config::InstallLnks()
{
	if( m_lnks.empty() && m_lnks_sudo.empty() ) return true;

	int ctr = 1;
	for( auto & lb : m_lnks ) {
		std::cout << "Info: Installing link[" << ctr << "] locations ...\n";
		for( auto & l : lb.lnks ) {
			auto src_loc = lb.prefix_src;
			if( * ( src_loc.end() - 1 ) != '/' ) src_loc += "/";
			src_loc += l.src;

			auto dest_loc = lb.prefix_dest;
			if( * ( dest_loc.end() - 1 ) != '/' ) dest_loc += "/";
			dest_loc += l.src;

			std::cout << "  => '" << src_loc << "' -> '" << dest_loc << "' ... ";
			if( Env::Exec( "ln -sf " + src_loc + " " + dest_loc ) == 0 ) {
				std::cout << "Done\n";
			} else {
				std::cout << "Failed\n";
				return false;
			}
		}
		++ctr;
	}

	ctr = 1;
	for( auto & lb : m_lnks_sudo ) {
		std::cout << "Info: Installing link[" << ctr << "] locations ...\n";
		for( auto & l : lb.lnks ) {
			auto src_loc = lb.prefix_src;
			if( * ( src_loc.end() - 1 ) != '/' ) src_loc += "/";
			src_loc += l.src;

			auto dest_loc = lb.prefix_dest;
			if( * ( dest_loc.end() - 1 ) != '/' ) dest_loc += "/";
			dest_loc += l.src;

			std::cout << "  => '" << src_loc << "' -> '" << dest_loc << "' ... ";
			if( Env::Exec( "sudo ln -sf " + src_loc + " " + dest_loc ) == 0 ) {
				std::cout << "Done\n";
			} else {
				std::cout << "Failed\n";
				return false;
			}
		}
		++ctr;
	}
	return true;
}

bool Config::PerfPostLnkCmds()
{
	if( m_final_cmds.empty() ) return true;

	std::string shell = Env::GetVarVal( "SHELL" );
	if( shell.empty() ) {
		std::cerr << "No SHELL variable found for executing post link commands!\n";
		return false;
	}
	std::cout << "Info: Executing Post link commands (using SHELL: " << shell << ") ...\n";
	for( auto & cmd : m_final_cmds ) {
		std::cout << "  => '" << shell + " -c '" + cmd + "'" << "' ... ";
		std::cout.flush();
		int err;
		if( ( err = Env::Exec( shell + " -c '" + cmd + "'" ) ) != 0 ) {
			std::cout << "Failed (" << err << ")\n";
			return false;
		} else {
			std::cout << "Success\n";
		}
	}
	return true;
}

static void FetchDirs( YAML::Node & conf, const std::string & conf_key, std::vector< std::string > & m_dirs )
{
	if( conf[ conf_key ] ) {
		for( auto dir : conf[ conf_key ] ) {
			m_dirs.push_back( Env::ExpandPathConst( dir.as< std::string >() ) );
		}
	}
	std::cout << "  => " << m_dirs.size() << " required directories";
	if( conf_key == "dirs_sudo" ) {
		std::cout << " that need sudo";
	}
	std::cout << "\n";
}

static void FetchLnks( YAML::Node & conf, const std::string & conf_key,
	std::vector< lnk_block > & m_lnks, const std::string & repo_dir )
{
	if( conf[ conf_key ] ) {
		lnk_block tmp_blk;
		bool finished_block = false;
		for( auto l : conf[ conf_key ] ) {
			if( l.IsMap() ) {
				auto m = l.begin();
				std::string key = m->first.as< std::string >();
				if( key == "prefix_" ) {
					if( finished_block ) {
						if( !tmp_blk.lnks.empty() ) {
							m_lnks.push_back( tmp_blk );

							tmp_blk.prefix_src.clear();
							tmp_blk.prefix_dest.clear();
							tmp_blk.lnks.clear();
						}
						finished_block = false;
					}
					auto prefix = m->second.as< std::vector< std::string > >();
					if( prefix.size() > 0 ) {
						tmp_blk.prefix_src = repo_dir + "/" + Env::ExpandPath( prefix[ 0 ] );
					}
					if( prefix.size() > 1 ) {
						tmp_blk.prefix_dest = Env::ExpandPath( prefix[ 1 ] );
					}
					finished_block = true;
					continue;
				}
				auto & src = key;
				auto dest = m->second.as< std::string >();
				tmp_blk.lnks.push_back( { Env::ExpandPath( src ), Env::ExpandPath( dest ) } );
				continue;
			}
			auto loc = Env::ExpandPathConst( l.as< std::string >() );
			tmp_blk.lnks.push_back( { loc, loc } );
		}
		if( !tmp_blk.lnks.empty() ) {
			m_lnks.push_back( tmp_blk );
		}
	}
	std::cout << "  => " << m_lnks.size() << " link block(s)";
	if( m_lnks.size() > 0 ) {
		std::cout << " with a total of ";
		int lnks = 0;
		for( auto & lnk_blk : m_lnks ) {
			lnks += lnk_blk.lnks.size();
		}
		std::cout << lnks << " links";
	}
	if( conf_key == "lnk_sudo" ) std::cout << " that need sudo";
	std::cout << "\n";
}