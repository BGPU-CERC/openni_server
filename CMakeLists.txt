cmake_minimum_required(VERSION 3.27)

set(TARGET openni_server)
project(${TARGET})

set(ASTRA_ROOT "${CMAKE_SOURCE_DIR}/ThirdParty/AstraSDK")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${ASTRA_ROOT}/samples/cmake")
find_package(Astra REQUIRED)

include_directories(
    "${CMAKE_SOURCE_DIR}/ThirdParty/Common"
    "${CMAKE_SOURCE_DIR}/ThirdParty/OpenNI2/Include"
	${ASTRA_INCLUDE_DIR}
)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	add_definitions(-DPLATFORM_WIN)
	MESSAGE(STATUS "IS Windows platform")
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		MESSAGE(STATUS "IS Windows x64")
		link_directories(
			"${CMAKE_SOURCE_DIR}/ThirdParty/OpenNI2/windows/x64-Release"
		)
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
		MESSAGE(STATUS "IS Windows x86")
		link_directories(
			"${CMAKE_SOURCE_DIR}/ThirdParty/OpenNI2/windows/Win32-Release"
		)
	endif()
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
	add_definitions(-DPLATFORM_LINUX)
	MESSAGE(STATUS "IS Linux platform")
	set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
	set(CMAKE_INSTALL_RPATH .)
	if(BUILD_PLATFORM MATCHES "x86_64")
		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			MESSAGE(STATUS "IS Linux x64")
			link_directories(
				"${CMAKE_SOURCE_DIR}/ThirdParty/OpenNI2/linux/x64"
			)
		elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
			MESSAGE(STATUS "IS Linux x86")
			link_directories(
				"${CMAKE_SOURCE_DIR}/ThirdParty/OpenNI2/linux/x86"
			)
		endif()
	elseif(BUILD_PLATFORM MATCHES "armv7l")
		MESSAGE(STATUS "IS armv7l")
		link_directories(
			"${CMAKE_SOURCE_DIR}/ThirdParty/OpenNI2/arm/Arm"
		)
	elseif(BUILD_PLATFORM MATCHES "aarch64")
		MESSAGE(STATUS "IS armv8l")
		link_directories(
			"${CMAKE_SOURCE_DIR}/ThirdParty/OpenNI2/arm/Arm64"
		)
	endif()
else()
endif()

file(GLOB SRC "*.h" "*.cpp")
add_executable(${TARGET} ${SRC})
target_link_libraries(${TARGET} OpenNI2 ${ASTRA_LIBRARIES})
