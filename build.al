project( dotter ) {
	version( 0.1 )
	language( cpp, 17 )
	license( bsd3 )
	author( Electrux, "ElectruxRedsworth@gmail.com" )
}

builds.add_cxx_flags( "-O2" )

load_file( yaml_cpp )
load_file( filesystem )

builds( bin ) {
	sources( "src/(.*)\.cpp" )
	build( dott, "src/main.cpp" )
}

builds( test ) {
	sources( "src/(.*)\.cpp", "-src/main.cpp" )
	build( Env_GetVarVal, "tests/Env_GetVarVal.cpp" )
	build( FileIO_LocExists, "tests/FileIO_LocExists.cpp" )
	build( Strings_Delimit, "tests/Strings_Delimit.cpp" )
	build( Config_LoadConfig, "tests/Config_LoadConfig.cpp" )
	build( Strings_IsOneOf, "tests/Strings_IsOneOf.cpp" )
	build( Config_CheckEnvVars, "tests/Config_CheckEnvVars.cpp" )
	build( Strings_ToVec, "tests/Strings_ToVec.cpp" )
	build( Env_ExpandPath, "tests/Env_ExpandPath.cpp" )
	runtests()
}
