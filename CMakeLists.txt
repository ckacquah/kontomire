cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "opengl_cpp_template_project")

project(${PROJECT_NAME} VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
    "Debug"
    CACHE
    STRING
    "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
endif()

message(STATUS "Started CMake for ${PROJECT_NAME} v${PROJECT_VERSION}...\n")

add_executable(
  ${PROJECT_NAME}
  ${PROJECT_SOURCE_DIR}/src/main.cpp
  ${PROJECT_SOURCE_DIR}/src/window.cpp
  ${PROJECT_SOURCE_DIR}/src/imgui_manager.cpp)

set(GLFW_INSTALL
  OFF
  CACHE BOOL "" FORCE)
set(GLFW_UNINSTALL
  OFF
  CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS
  OFF
  CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS
  OFF
  CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES
  OFF
  CACHE BOOL "" FORCE)
set(GLFW_DOCUMENT_INTERNALS
  OFF
  CACHE BOOL "" FORCE)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/deps)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(${PROJECT_SOURCE_DIR}/deps/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/deps/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/deps/imgui)
add_subdirectory(${PROJECT_SOURCE_DIR}/deps/implot)

target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} implot)

# ####################################################
# ####################################################
# ################### TESTS ##########################
# ####################################################
# ####################################################
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/deps/googletest)

enable_testing()

add_executable(hello_test ${PROJECT_SOURCE_DIR}/tests/hello_test.cc)
target_link_libraries(hello_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(hello_test)