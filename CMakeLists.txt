# ======================================================================
#  icartdaq_core main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/artdaq-demo/ups/setup_for_development
#  buildtool
# ======================================================================

cmake_minimum_required (VERSION 3.19 FATAL_ERROR)

find_package(cetmodules 3.16.00 REQUIRED)

project(sbndaq_artdaq_core VERSION 1.08.06)

include(CetCMakeEnv)
cet_cmake_env()

SET(ICARUS_BUILD 1)

cet_set_compiler_flags(DIAGS VIGILANT
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic
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

message("Searching ${sbndaq_artdaq_core_EXEC_PREFIX} for icarus or sbnd qualifiers")
string(FIND "x${sbndaq_artdaq_core_EXEC_PREFIX}" "icarus" ICARUS_FOUND)
string(FIND "x${sbndaq_artdaq_core_EXEC_PREFIX}" "sbnd" SBND_FOUND)

IF (ICARUS_FOUND EQUAL -1)
   MESSAGE ("No ICARUS_BUILD?")
ELSE()
   SET(ICARUS_BUILD 1)
   MESSAGE ("Performing ICARUS_BUILD")
ENDIF()

IF (SBND_FOUND EQUAL -1)
   MESSAGE ("No SBND_BUILD?")
ELSE()
   SET(SBND_BUILD 1)
   MESSAGE ("Performing SBND_BUILD")
ENDIF()

IF (NOT;;;(;;;SBND_BUILD OR ICARUS_BUILD;;;))
   MESSAGE("No experiment-specific build specified. Building all.")
   SET(SBND_BUILD 1)
   SET(ICARUS_BUILD 1)
ENDIF()


find_package( artdaq_core QUIET REQUIRED EXPORT)
find_package( TRACE QUIET REQUIRED EXPORT)
find_package( cetlib QUIET REQUIRED EXPORT )
find_package( cetlib_except QUIET REQUIRED EXPORT)


#experiment specific products needed
IF(ICARUS_BUILD)

ENDIF()

IF(SBND_BUILD)

ENDIF()

# macros for art_dictionary and basic_plugin
include(CetTest)
include(ArtDictionary)

# source
add_subdirectory(sbndaq-artdaq-core)

cet_cmake_config()
