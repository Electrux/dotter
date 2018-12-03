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
static void FetchLnks( YAML::Node & conf, const std::string & conf_key, std::vector< lnk_block > & m_lnks );

bool Config::LoadConfig( const std::string & file_name )
{
	if( !FS::LocExists( file_name ) ) {
		std::cerr << "Unable to open configuration file: " << file_name << "\n";
		return false;
	}

	YAML::Node conf = YAML::LoadFile( file_name );

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
	std::cout << "Info: Loaded " << m_env_vars.size() << " env vars!\n";

	// Now, fetch the directories and sudo directories to be created
	FetchDirs( conf, "dirs", m_dirs );
	FetchDirs( conf, "dirs_sudo", m_dirs_sudo );

	// Now, retrieve the links and sudo links
	FetchLnks( conf, "lnk", m_lnks );
	FetchLnks( conf, "lnk_sudo", m_lnks_sudo );

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
	std::cout << "Info: Loaded " << m_final_cmds.size() << " post link commands!\n";

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
	for( auto & v : m_env_vars ) {
		std::cerr << "Is set environment variable: '" << v.first << "' ... ";
		std::string val = Env::GetVarVal( v.first );

		if( val.empty() ) {
			std::cerr << "No\n";
			return false;
		}

		std::cerr << "Yes";
		if( !v.second.empty() ) {
			int pos = Str::IsOneOf( v.second, val );
			std::cerr << " (" << pos << ")\n";
			if( pos < 0 ) return false;
		}
		std::cerr << "\n";
	}
	return true;
}

static void FetchDirs( YAML::Node & conf, const std::string & conf_key, std::vector< std::string > & m_dirs )
{
	if( conf[ conf_key ] ) {
		for( auto dir : conf[ conf_key ] ) {
			m_dirs.push_back( dir.as< std::string >() );
		}
	}
	std::cout << "Info: Loaded " << m_dirs.size() << " required directories";
	if( conf_key == "dirs_sudo" ) {
		std::cout << " that need sudo";
	}
	std::cout << "!\n";
}

static void FetchLnks( YAML::Node & conf, const std::string & conf_key, std::vector< lnk_block > & m_lnks )
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
						tmp_blk.prefix_src = prefix[ 0 ];
					}
					if( prefix.size() > 1 ) {
						tmp_blk.prefix_dest = prefix[ 1 ];
					}
					finished_block = true;
					continue;
				}
				auto & src = key;
				auto dest = m->second.as< std::string >();
				tmp_blk.lnks.push_back( { src, dest } );
				continue;
			}
			auto loc = l.as< std::string >();
			tmp_blk.lnks.push_back( { loc, loc } );
		}
		if( !tmp_blk.lnks.empty() ) {
			m_lnks.push_back( tmp_blk );
		}
	}
	std::cout << "Info: Loaded " << m_lnks.size() << " link block(s)";
	if( m_lnks.size() > 0 ) {
		std::cout << " with a total of ";
		int lnks = 0;
		for( auto & lnk_blk : m_lnks ) {
			lnks += lnk_blk.lnks.size();
		}
		std::cout << lnks << " links";
	}
	if( conf_key == "lnk_sudo" ) std::cout << " that need sudo";
	std::cout << "!\n";
}