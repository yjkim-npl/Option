#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.8...3.18)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(Option)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
set(ACTIVATE_EVE OFF CACHE INTERNAL "")
if(ACTIVATE_EVE)
    add_definitions(-DACTIVATE_EVE)
endif(ACTIVATE_EVE)
if(ACTIVATE_EVE)
    find_package(ROOT REQUIRED COMPONENTS RIO Net Eve EG RGL Geom Gui Graf Graf3d Ged Matrix MathCor    e)
else(ACTIVATE_EVE)
    find_package(ROOT REQUIRED COMPONENTS RIO Net Geom Gui Graf Graf3d Ged Matrix MathCore)
endif(ACTIVATE_EVE)

include(${ROOT_USE_FILE})
#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(Opsim Opsim.cc ${sources} ${headers})
target_link_libraries(Opsim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(OPTION_SCRIPTS
  OpParameter.conf
  init_vis.mac
  run1.mac
  run2.mac
  vis.mac
  )

foreach(_script ${OPTION_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

##----------------------------------------------------------------------------
## For internal Geant4 use - but has no effect if you build this
## example standalone
##
#add_custom_target(Option DEPENDS Option)
#
##----------------------------------------------------------------------------
## Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
##
#install(TARGETS Option DESTINATION bin)
