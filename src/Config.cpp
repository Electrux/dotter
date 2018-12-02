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
				std::cerr << "Else: " << v.as< std::string >() << "\n";
				key = v.as< std::string >();
			}
			m_env_vars[ key ] = val;
		}
	}
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
		std::cout << "Is set environment variable: '" << v.first << "' ... ";
		std::string val = Env::GetVarVal( v.first );

		if( val.empty() ) {
			std::cout << "No\n";
			return false;
		}

		std::cout << "Yes";
		if( !v.second.empty() ) {
			int pos = Str::IsOneOf( v.second, val );
			std::cout << " (" << pos << ")\n";
			if( pos < 0 ) return false;
		}
	}
	return true;
}