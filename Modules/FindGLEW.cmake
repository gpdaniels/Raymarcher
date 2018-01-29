# - Try to find the GLEW library
# Once done this will define
#
#  GLEW_FOUND - system has GLEW
#  GLEW_INCLUDE_DIRS - the GLEW include directory
#  GLEW_LIBRARIES - The libraries needed to use GLEW
 
if(GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES)
   set(GLEW_FOUND TRUE)
else(GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES)

SET(GLEW_DIR "/Users/gpdaniels/Repositories/glew-2.1.0")

FIND_PATH(GLEW_INCLUDE_DIRS GL/glew.h
   /usr/include
   /usr/local/include
   $ENV{GLEWROOT}/include
   $ENV{GLEW_ROOT}/include
   ${GLEW_DIR}/include
   ${GLEW_DIR}/inc
   [HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\8.0\\Setup\\VC]/PlatformSDK/Include
)
 
FIND_LIBRARY(GLEW_LIBRARIES NAMES glew64 GLEW glew glew32
   PATHS
   /usr/lib
   /usr/lib64
   /usr/local/lib
   /usr/local/lib64
   $ENV{GLEWROOT}/lib
   $ENV{GLEW_ROOT}/lib
   ${GLEW_DIR}/lib
   [HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\8.0\\Setup\\VC]/PlatformSDK/Lib
   DOC "glew library name"
)
 
if(GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES)
   set(GLEW_FOUND TRUE)
endif(GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES)
 
 
if(GLEW_FOUND)
   if(NOT GLEW_FIND_QUIETLY)
      message(STATUS "Found GLEW: ${GLEW_LIBRARIES}")
   endif(NOT GLEW_FIND_QUIETLY)
else(GLEW_FOUND)
   if(GLEW_FIND_REQUIRED)
      message(FATAL_ERROR "could NOT find glew")
   endif(GLEW_FIND_REQUIRED)
endif(GLEW_FOUND)
 
MARK_AS_ADVANCED(GLEW_INCLUDE_DIRS GLEW_LIBRARIES)
 
endif(GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES)
