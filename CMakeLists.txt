#-------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.8)
project(Option)

#-------------------------------------------------------
set(ACTIVATE_EVE OFF CACHE INTERNAL "")
if(ACTIVATE_EVE)
	add_definition(-DACTIVATE_EVE)
endif(ACTIVATE_EVE)
if(ACTIVATE_EVE)
  find_package(ROOT REQUIRED COMPONENTS RIO Net Eve EG RGL Geom Gui Graf Graf3d Ged Matrix MathCore)
else(ACTIVATE_EVE)
  find_package(ROOT REQUIRED COMPONENTS RIO Net Geom Gui Graf Graf3d Ged Matrix MathCore)
endif(ACTIVATE_EVE)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
set(useROOT true)
if(useROOT)
	list(APPEND ${PROJECT_SOURCE_DIR}/src)
	list(APPEND ${PROJECT_SOURCE_DIR}/include)
	list(APPEND ${PROJECT_SOURCE_DIR}/Option.cc)
#	EXECUTE_PROCESS()
endif(useROOT)
include(${ROOT_USE_FILE})
#set(TOOLS_LIBRARIES_LIST ${ROOT_LIBRARIES} CACHE INTERNAL "")
link_directories(${ROOT_LIBRARIES_DIR})
#-------------------------------------------------------
option(WITH_GEANT4_UIVIS "Build the Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
	find_package(Geant4 REQUIRED ui_all vis_all)
else()
	find_package(Geant4 REQUIRED)
endif()

#-------------------------------------------------------
include(${Geant4_USE_FILE})

#-------------------------------------------------------
include_directories(${PROJECT_SOURCE_DIR}/include ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

#-------------------------------------------------------
add_executable(Option Option.cc ${sources} ${headers})
target_link_libraries(Option ${Geant4_LIBRARIES} ${TOOLS_LIBRARIES_LIST})

#-------------------------------------------------------
set(Option_lists
	vis.mac
	#...
	)

foreach(_file ${Option_lists})
	configure_file(
	${PROJECT_SOURCE_DIR}/${_file}
	${PROJECT_BINARY_DIR}/${_file}
	COPYONLY
	)
endforeach()
#-------------------------------------------------------
install(TARGETS Option DESTINATION bin)
