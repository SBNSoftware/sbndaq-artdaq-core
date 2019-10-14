# ======================================================================
#  icartdaq_core main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/artdaq-demo/ups/setup_for_development
#  buildtool
# ======================================================================



# use cmake 3.7 or later
cmake_minimum_required (VERSION 3.7)

project(sbndaq-artdaq-core)

#optional build setup

string(FIND $ENV{MRB_QUALS} "icarus" ICARUS_FOUND)
IF (ICARUS_FOUND EQUAL -1)
   MESSAGE ("No ICARUS_BUILD?")
ELSE()
   SET(ICARUS_BUILD 1)
   MESSAGE ("Performing ICARUS_BUILD")
ENDIF()

string(FIND $ENV{MRB_QUALS} "sbnd" SBND_FOUND)
IF (SBND_FOUND EQUAL -1)
   MESSAGE ("No SBND_BUILD?")
ELSE()
   SET(SBND_BUILD 1)
   MESSAGE ("Performing SBND_BUILD")
ENDIF()

IF (NOT(SBND_BUILD OR ICARUS_BUILD))
   MESSAGE("No experiment-specific build specified. Building all.")
   SET(SBND_BUILD 1)
   SET(ICARUS_BUILD 1)
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
 EXTRA_FLAGS -pedantic -pthread
 )

SET ( SANITIZE_ADDRESS $ENV{SANITIZE_ADDRESS} )
IF ( SANITIZE_ADDRESS )
    MESSAGE (STATUS "Enable  -fsanitize=address -Wno-unused-command-line-argument -fno-omit-frame-pointer")
    cet_add_compiler_flags(-fsanitize=address -Wno-unused-command-line-argument -fno-omit-frame-pointer)
ENDIF()

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
#find_ups_product( icarus_artdaq_base v0_08_04 )
find_ups_product( artdaq_core v3_05_07)
find_ups_product( TRACE v3_15_03 )
find_ups_product( cetlib v3_07_02 )
find_ups_product( cetlib_except v1_03_03 )
find_ups_boost( v1_69_0 filesystem
                        program_options
                        unit_test_framework
 )


find_ups_product( cetbuildtools v7_13_02 )

#experiment specific products needed
IF(ICARUS_BUILD)
  find_ups_product( icarus_artdaq_base  v0_09_00)
ENDIF()

IF(SBND_BUILD)

ENDIF()

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
