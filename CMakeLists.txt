# ======================================================================
#  icartdaq_core main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/artdaq-demo/ups/setup_for_development
#  buildtool
# ======================================================================

cmake_minimum_required (VERSION 3.18.2)

find_package(cetmodules)
set(CET_CETBUILDTOOLS_COMPAT TRUE)
include(Compatibility)
set(CET_WARN_DEPRECATED)

project(sbndaq_artdaq_core VERSION 0.09.00)

set(CMAKE_MODULE_PATH
  $ENV{ARTDAQ_UTILITIES_DIR}/Modules
  ${CMAKE_MODULE_PATH})
include(CetCMakeEnv)

cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -DRTI_UNIX -DRTI_LINUX -DRTI_64BIT -m64
)

string(TOUPPER ${CMAKE_BUILD_TYPE} BTYPE_UC)
if(BTYPE_UC STREQUAL DEBUG)
  add_definitions(-DDEBUGME)
  if(DEFINED ENV{USE_ASAN})
    if($ENV{USE_ASAN} GREATER 0)
      message("!!! Address Sanitizer Activated!!!")
      add_compile_options(-fsanitize=address -Wno-unused-command-line-argument)
      add_link_options(-lasan)
    endif()
  endif()
  if(DEFINED ENV{USE_TSAN})
    if($ENV{USE_TSAN} GREATER 0)
      message("!!! Thread Sanitizer Activated!!!")
      add_compile_options(-fsanitize=thread -Wno-unused-command-line-argument)
      add_link_options(-ltsan)
    endif()
  endif()
  if(DEFINED ENV{USE_GCOV})
    add_compile_options(-fprofile-arcs -ftest-coverage --coverage)
    add_link_options(--coverage)
  endif()
endif()

#exeption stack trace
if(DEFINED ENV{USE_EXCEPTIONSTACKTRACE})
  if($ENV{USE_EXCEPTIONSTACKTRACE} GREATER 0)
    message("!!! Exception StackTrace Activated!!!")
    add_definitions(-D EXCEPTIONSTACKTRACE)
    set( EXCEPTIONSTACKTRACE_LIB  artdaq-core_Utilities_ExceptionStackTrace )
    add_compile_options( -rdynamic -Wno-unused-command-line-argument )
    add_link_options( -rdynamic )
  endif()
endif()

if(DEFINED $ENV{MRB_QUALS})
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
endif()

IF (NOT(SBND_BUILD OR ICARUS_BUILD))
   MESSAGE("No experiment-specific build specified. Building all.")
   SET(SBND_BUILD 1)
   SET(ICARUS_BUILD 1)
ENDIF()


# these are minimum required versions, not the actual product versions
find_ups_product( messagefacility )
find_ups_product( artdaq_core )
find_ups_product( TRACE )
find_ups_product( cetlib v3_13_01 )
find_ups_product( cetlib_except v1_07_01 )
find_ups_boost( v1_73_0 filesystem
                        program_options
                        unit_test_framework
 )


find_ups_product( cetbuildtools v8_13_04 )

#experiment specific products needed
IF(ICARUS_BUILD)

ENDIF()

IF(SBND_BUILD)

ENDIF()

# macros for art_dictionary and simple_plugin
#include(ArtMake)
include(CetTest)

# source
add_subdirectory(sbndaq-artdaq-core)

cet_cmake_config()
