cmake_minimum_required(VERSION 2.8.11)

project(libMidiPlayer)

configure_file(
  ${CMAKE_SOURCE_DIR}/version.cmake.in
  ${CMAKE_BINARY_DIR}/version.cmake
)
include_directories(${CMAKE_BINARY_DIR})
include(${CMAKE_BINARY_DIR}/version.cmake)

# check compiler
# if visual studio >= vc12.0
if(MSVC AND ((MSVC_VERSION GREATER 1800) OR (MSVC_VERSION EQUAL 1800)) )
	message(STATUS "Your compiler is managed")
elseif(CMAKE_COMPILER_IS_GNUCXX)
	message(STATUS "Your compiler is managed")
else()
	message(FATAL_ERROR "Your compiler is not managed by this CMakeLists. Only MSVC12 or GCC (G++) is managed")
endif()

option(INSTALL_WITH_CPACK "Make deployment on Windows installer or Linux DEB file" OFF )

if(CMAKE_CL_64 OR CMAKE_SIZEOF_VOID_P EQUAL 8 )
	SET(ARCH "x64")
else()
	SET(ARCH "x86")
endif()
MESSAGE(STATUS "ARCH = ${ARCH}")

if(MSVC)
	option(NO_CONSOLE_MODE "Have a console to debuging" OFF)
	if(ARCH STREQUAL "x64" ) 
		set(QT5_DIR "D:/Qt/Qt5.9.1/msvc2017_64/lib/cmake" CACHE STRING "Additional search paths for QT5")
		add_definitions(-D__ARCH64__)
	else()
		set(QT5_DIR "D:/Qt/Qt5.9.1/msvc2015/lib/cmake" CACHE STRING "Additional search paths for QT5")
	endif()
	find_path(Qt5Widgets_DIR Qt5WidgetsConfig.cmake PATHS "${QT5_DIR}/Qt5Widgets" )
	find_path(Qt5Network_DIR Qt5NetworkConfig.cmake PATHS "${QT5_DIR}/Qt5Network" )
	find_path(Qt5Xml_DIR     Qt5XmlConfig.cmake     PATHS "${QT5_DIR}/Qt5Xml"     )
	add_definitions(/W3)
	add_definitions(-D__WINDOWS_MM__ -D_UNICODE)
	if(NO_CONSOLE_MODE)
		add_definitions(-DNO_CONSOLE_MODE)
	endif()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") # multi processor compiliation.
	
else()
    add_definitions(-std=c++11)
	add_definitions(-D__LINUX_ALSASEQ__)
	add_definitions(-D__LINUX_ALSA__)
	set(QT5_DIR "~/Qt5.9.1/5.9.1/gcc_64/lib/cmake" CACHE STRING "Additional search paths for QT5")
	find_path(Qt5Widgets_DIR Qt5WidgetsConfig.cmake PATHS "${QT5_DIR}/Qt5Widgets" )
	find_path(Qt5Network_DIR Qt5NetworkConfig.cmake PATHS "${QT5_DIR}/Qt5Network" )
	find_path(Qt5Xml_DIR     Qt5XmlConfig.cmake     PATHS "${QT5_DIR}/Qt5Xml"     )
endif()

# Avoid Unicode -> QString issues. 
add_definitions(-DQT_NO_CAST_TO_ASCII)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Xml     REQUIRED)

# Turn on automatic invocation of the MOC
set(CMAKE_AUTOMOC ON)
#add_definitions(${Qt5Core_DEFINITIONS})
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")


file( GLOB MidiEvent ../src/MidiEvent/*.cpp  )
file( GLOB Midi      ../src/midi/*.cpp       )

file( GLOB MidiEventh ../src/MidiEvent/*.h  )
file( GLOB Midih      ../src/midi/*.h       )

source_group( "MidiEvent"      FILES ${MidiEvent} )
source_group( "MidiEvent\\inc" FILES ${MidiEventh})
source_group( "midi"           FILES ${Midi}      )
source_group( "midi\\inc"      FILES ${Midih}     )
source_group( ""               FILES ${main} ${mainh} )

#add_executable(libMidiPlayer
#               ${main}      ${mainh}
#               ${RTMidi}    ${RTMidih}
#               ${Protocol}  ${Protocolh}
#               ${Midi}      ${Midih}
#               ${gui}       ${guih}
#               ${MidiEvent} ${MidiEventh}
#               ${Remote}    ${Remoteh}
#               ${Tool}      ${Toolh} midieditor.rc)

#dynamic library  
#ADD_LIBRARY(libMidiPlayer SHARED ${LIB_SRC})  
  
#static library  
ADD_LIBRARY(libMidiPlayer  ${Midi} ${MidiEvent})  

#include(ressources.cmake)
set(libMidiPlayerDep Qt5::Widgets Qt5::Network Qt5::Xml)

# prepare outup directory 
set(NAME_RELEASE MidiEditor_${ARCH}_${GIT_TAG})
set(DIR_NAME_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/${NAME_RELEASE})
set_target_properties( libMidiPlayer PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${DIR_NAME_RELEASE})
#add_dependencies(libMidiPlayer ressources)
#add_custom_command(TARGET ressources POST_BUILD   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/ressources.rcc ${DIR_NAME_RELEASE}/ressources.rcc )
#add_custom_command(TARGET libMidiPlayer POST_BUILD 
#                   COMMAND ${QT5_DIR}/../../bin/windeployqt ${DIR_NAME_RELEASE}
#				   COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${NAME_RELEASE}.zip" --format=zip ${DIR_NAME_RELEASE} )
#file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/run_environment/metronome/metronome-01.wav" DESTINATION ${DIR_NAME_RELEASE}/metronome)

if(MSVC)
	#target_link_libraries(libMidiPlayer Winmm.lib ${libMidiPlayerDep})
	target_link_libraries(libMidiPlayer ${libMidiPlayerDep})
	
	if(NO_CONSOLE_MODE)
		#set_target_properties(libMidiPlayer PROPERTIES  LINK_FLAGS "/SUBSYSTEM:WINDOWS")
	endif()
    set(QT5_DLLs_DEP_RELEASE "${QT5_DIR}/../../bin/Qt5Widgets.dll"
                             "${QT5_DIR}/../../bin/Qt5Network.dll"
                             "${QT5_DIR}/../../bin/Qt5Xml.dll"
                             "${QT5_DIR}/../../bin/Qt5Gui.dll"
                             "${QT5_DIR}/../../bin/Qt5Core.dll")
    set(QT5_DLLs_DEP_DEBUG   "${QT5_DIR}/../../bin/Qt5Widgetsd.dll"
                             "${QT5_DIR}/../../bin/Qt5Networkd.dll"
                             "${QT5_DIR}/../../bin/Qt5Xmld.dll"
                             "${QT5_DIR}/../../bin/Qt5Guid.dll"
                             "${QT5_DIR}/../../bin/Qt5Cored.dll")
    file(COPY ${QT5_DLLs_DEP_RELEASE} ${QT5_DLLs_DEP_DEBUG} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else()
	target_link_libraries(libMidiPlayer asound sfml-system sfml-audio pthread ${libMidiPlayerDep})
endif()
add_custom_command(TARGET libMidiPlayer PRE_BUILD COMMAND cmake -P ${CMAKE_BINARY_DIR}/../version.cmake)

#file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/run_environment/libMidiPlayer.ico" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/run_environment/metronome/metronome-01.wav" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/metronome)

if(INSTALL_WITH_CPACK)
	install(DIRECTORY "${DIR_NAME_RELEASE}/" DESTINATION bin)
	if(WIN32)#Windows
		set(CPACK_GENERATOR NSIS)
		set(CPACK_PACKAGE_INSTALL_DIRECTORY "libMidiPlayer")
		set(CPACK_PACKAGE_EXECUTABLES "libMidiPlayer" "libMidiPlayer" ${CPACK_PACKAGE_EXECUTABLES}) 
		set(CPACK_NSIS_MODIFY_PATH OFF)
	else() #UNIX
		set(CPACK_GENERATOR "DEB")
		SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Amaury Bréhéret")
		set (CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5widgets5:amd64 (>=5.7), libqt5network5:amd64 (>=5.7), libqt5xml:amd64 (>=5.7), libsfml-system,libsfml-audio,libasound,libpthread")
		#set(CPACK_DEBIAN_PACKAGE_DEPENDS "qt5-default (>= 5.7),libsfml-system,libsfml-audio,libasound,libpthread") 
		#set(CPACK_DEBIAN_PACKAGE_DEPENDS "libQt5Widgets,libQt5Network,libQt5Xml,libQt5Core,libsfml-system,libsfml-audio,libasound,libpthread")
		set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
	endif()
	set(CPACK_PACKAGE_CONTACT "Amaury Bréhéret abreheret@gmail.com")
	set(CPACK_PACKAGE_NAME "libMidiPlayer")
	set(CPACK_PACKAGE_VENDOR "https://github.com/abreheret/libMidiPlayer")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libMidiPlayer - Installation ")
	set(CPACK_PACKAGE_VERSION ${GIT_TAG})
	INCLUDE(CPack)
endif()
#set( CMAKE_BUILD_TYPE Debug )