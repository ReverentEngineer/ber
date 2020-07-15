cmake_minimum_required(VERSION 2.8)
enable_testing()

project(ber C)
set(PROJECT_VERSION 1.0.0)

include(CTest)

if (APPLE)
  set(CMAKE_MACOSX_RPATH FALSE)
endif()

include_directories(src)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(doc)
add_subdirectory(examples)

set(CPACK_GENERATOR "TGZ")

find_program(RPM_EXECUTABLE rpm)
if (RPM_EXECUTABLE)
  list(APPEND CPACK_GENERATOR "RPM")
endif(RPM_EXECUTABLE)

find_program(DPKG_EXECUTABLE dpkg)
if (DPKG_EXECUTABLE)
  list(APPEND CPACK_GENERATOR "DEB")
endif(DPKG_EXECUTABLE)

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_RELEASE 1)
include(CPack)
