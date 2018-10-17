# ======================================================================
#  icartdaq_core main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/artdaq-demo/ups/setup_for_development
#  buildtool
# ======================================================================


project(sbndaq-artdaq-core)

# use cmake 3.7 or later
cmake_minimum_required (VERSION 3.7)

#optional for icarus build

string(FIND $ENV{MRB_QUALS} "icarus" ICARUS_FOUND)
IF (ICARUS_FOUND EQUAL -1)
   MESSAGE ("No ICARUS_BUILD")
ELSE()
   SET(ICARUS_BUILD 1)
   MESSAGE ("Performing ICARUS_BUILD")
ENDIF()

string(FIND $ENV{MRB_QUALS} "sbnd" SBND_FOUND)
IF (SBND_FOUND EQUAL -1)
   MESSAGE ("No SBND_BUILD")
ELSE()
   SET(SBND_BUILD 1)
   MESSAGE ("Performing SBND_BUILD")
ENDIF()

# cetbuildtools contains our cmake modules
SET ( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
IF (NOT CETBUILDTOOLS_VERSION)
    MESSAGE (FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules")
ENDIF()

set(CMAKE_MODULE_PATH 
        $ENV{ART_DIR}/Modules
        $ENV{CETBUILDTOOLS_DIR}/Modules
        ${CMAKE_MODULE_PATH})


include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT
 WERROR
 NO_UNDEFINED
 EXTRA_FLAGS -pedantic 
 )

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
#find_ups_product( icarus_artdaq_base v0_03_00 )
find_ups_product( artdaq_core v3_00_00 )
find_ups_product( TRACE v3_09_01 )
find_ups_product( cetlib v3_00_01 )
find_ups_product( cetlib_except v1_01_02 )
find_ups_boost( v1_63_0b filesystem
                        program_options
                        unit_test_framework
 )


find_ups_product( cetbuildtools v5_08_01 )
 
# macros for art_dictionary and simple_plugin
#include(ArtMake)
include(CetTest)

# source
add_subdirectory(sbndaq-artdaq-core)

# JCF, 8/19/14
# As of yet, no tests exist for this package

# testing
# add_subdirectory(test)

# ups - table and config files
add_subdirectory(ups)

# packaging utility
include(UseCPack)
