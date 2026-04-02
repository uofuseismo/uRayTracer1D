cmake_minimum_required(VERSION 3.24)
project(uRayTracer1D VERSION 0.0.1 LANGUAGES CXX)
enable_testing()

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(BUILD_TESTS "Compile unit tests" ON) 
option(USE_CLANG_TIDY "Build using clang-tidy" ON)

# Boost
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()
find_package(Boost COMPONENTS headers REQUIRED)
find_package(Catch2 3 REQUIRED)
find_package(TBB COMPONENTS tbb REQUIRED)

if (${USE_CLANG_TIDY} AND PROJECT_IS_TOP_LEVEL)
   message("Looking for clang-tidy")
   find_program(CLANG_TIDY_EXECUTABLE NAMES "clang-tidy")
   if (CLANG_TIDY_EXECUTABLE)
      message("Using clang-tidy  ${CLANG_TIDY_EXECUTABLE}")
      set(CMAKE_C_CLANG_TIDY   ${CLANG_TIDY_EXECUTABLE} -p ${CMAKE_BINARY_DIR})
      set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXECUTABLE} -p ${CMAKE_BINARY_DIR})
      set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 
   else()
      message("clang-tidy not found")
   endif()
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.hpp.in
               ${CMAKE_CURRENT_SOURCE_DIR}/include/uRayTracer1D/version.hpp @ONLY)

set(LIBRARY_SRC
    src/version.cpp
    src/receiver.cpp
    src/source.cpp
    src/acoustic/constantLayer.cpp)

if (${BUILD_SHARED_LIBS})
   message("Will build shared library")
   add_library(libuRayTracer1D SHARED ${LIBRARY_SRC})
else()
   message("Will build static library")
   add_library(libuRayTracer1D STATIC ${LIBRARY_SRC})
endif()
add_library(uRayTracer1D::libuRayTracer1D ALIAS libuRayTracer1D)
target_sources(libuRayTracer1D
               PUBLIC
               FILE_SET HEADERS
               BASE_DIRS
                  include
               FILES
                  include/uRayTracer1D/source.hpp
                  include/uRayTracer1D/version.hpp
               )
target_link_libraries(libuRayTracer1D
                      PUBLIC TBB::tbb
                      PRIVATE Boost::headers)

target_include_directories(libuRayTracer1D
                           PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
                           PUBLIC  $<INSTALL_INTERFACE:>)
set_target_properties(libuRayTracer1D PROPERTIES
                      CXX_STANDARD 20
                      CXX_STANDARD_REQUIRED YES 
                      CXX_EXTENSIONS NO
                      OUTPUT_NAME uRayTracer1D)
#                      POSITION_INDEPENDENT_CODE TRUE)
#########################################################################################
#                                        Unit Tests                                     #
#########################################################################################
if (PROJECT_IS_TOP_LEVEL AND ${BUILD_TESTS})
   add_executable(unitTests
                  testing/receiver.cpp
                  testing/source.cpp
                  testing/acoustic/model.cpp)
   set_target_properties(unitTests PROPERTIES
                         CXX_STANDARD 20
                         CXX_STANDARD_REQUIRED YES
                         CXX_EXTENSIONS NO)
   target_link_libraries(unitTests
                         PRIVATE TBB::tbb uRayTracer1D::libuRayTracer1D
                                 Catch2::Catch2 Catch2::Catch2WithMain)
   target_include_directories(unitTests
                              PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                              PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
   add_test(NAME unitTests
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testing
            COMMAND $<TARGET_FILE:unitTests>)
endif()

#========================================================================================#
#                                      Installation                                      #
#========================================================================================#
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
configure_package_config_file(
    cmake/${PROJECT_NAME}Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
)
install(TARGETS libuRayTracer1D
        EXPORT ${PROJECT_NAME}Targets
        FILE_SET HEADERS
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT libraries)
export(EXPORT ${PROJECT_NAME}Targets
       NAMESPACE uRayTracer1D::
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
##########################################################################################
#                                     CPack Packaging                                    #
##########################################################################################
include(CPackComponent)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "UUSS")
set(CPACK_PACKAGE_CONTACT "ben.baker@utah.edu")
set(CPACK_PACKAGE_LICENSE "MIT")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A seismic ray tracer for 1D layered media.")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
cpack_add_component(libraries
                    DISPLAY_NAME "Application common library"
                    DEPENDS TBB::tbb)
cpack_add_component(headers
                    DISPLAY_NAME "libuRayTracer1D header files"
                    DEPENDS libraries)
#cpack_add_component(applications
#                    DISPLAY_NAME "uRayTracer Applications"
#                    DEPENDS libraries)
set(CPACK_GENERATOR ZIP TGZ)
if (WIN32)
   list(APPEND CPACK_GENERATOR WIX)
elseif (APPLE)
   list(APPEND CPACK_GENERATOR productbuild)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
   find_program(RPMBUILD_PROGRAM rpmbuild)
   if (RPMBUILD_PROGRAM)
      list(APPEND CPACK_GENERATOR RPM)
      #set(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES abc >= 123")
   endif()
   if (${BUILD_FOR_DEBIAN})
      find_program(DPKG_PROGRAM dpkg REQUIRED)
   else()
      find_program(DPKG_PROGRAM dpkg)
   endif()
   if (DPKG_PROGRAM)
      list(APPEND CPACK_GENERATOR DEB)
      set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS} libtbb-dev (>= 2022.3.0-2)")
      set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
   endif()
else()
   message("Unknown operating system")
endif()
message("CPack generators " ${CPACK_GENERATOR})
set(CPACK_SOURCE_IGNORE_FILES
  /\\.git/
  \\.swp
  \\.orig
  /CMakeLists\\.txt\\.user
  /private/
)
include(CPack) # Put this last!
