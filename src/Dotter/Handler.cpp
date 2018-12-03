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

#include "../../include/Dotter/Get.hpp"
#include "../../include/Dotter/Use.hpp"
#include "../../include/Dotter/Handler.hpp"

int Dot::Handle( const std::vector< std::string > & args )
{
	if( args.size() < 2 ) {
		std::cout << "Usage: " << args[ 0 ] << " [get/use]\n";
		return -1;
	}
	if( args[ 1 ] == "get" ) {
		return Dot::Get( args );
	} else if( args[ 1 ] == "use" ) {
		return Dot::Use( args );
	}
	return 0;
}