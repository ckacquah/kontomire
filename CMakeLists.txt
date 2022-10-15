cmake_minimum_required(VERSION 3.10)

set(DEMO_APP "kontomire-demo")
set(KONTOMIRE_RENDERER "kontomire")

project(${KONTOMIRE_RENDERER} VERSION 1.0)

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

message(STATUS "Started CMake for ${DEMO_APP} v${PROJECT_VERSION}...\n")

# Project target
add_library(${KONTOMIRE_RENDERER}
  ${PROJECT_SOURCE_DIR}/src/kontomire/core/buffer.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/core/renderAPI.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/core/renderer2d.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/core/framebuffer.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/core/vertex_arrays.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/platform/opengl/buffer.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/platform/opengl/renderAPI.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/platform/opengl/framebuffer.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/platform/opengl/vertex_arrays.cc
)

target_include_directories(${KONTOMIRE_RENDERER} PUBLIC ${PROJECT_SOURCE_DIR}/deps)
target_include_directories(${KONTOMIRE_RENDERER} PUBLIC ${PROJECT_SOURCE_DIR}/include)

# The demo application
add_executable(
  ${DEMO_APP}
  ${PROJECT_SOURCE_DIR}/demo/src/main.cc
  ${PROJECT_SOURCE_DIR}/demo/src/window.cc
  ${PROJECT_SOURCE_DIR}/demo/src/imgui_manager.cc)

target_include_directories(${DEMO_APP} PUBLIC ${PROJECT_SOURCE_DIR}/deps)
target_include_directories(${DEMO_APP} PUBLIC ${PROJECT_SOURCE_DIR}/include)

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

add_subdirectory(${PROJECT_SOURCE_DIR}/deps/glm)
add_subdirectory(${PROJECT_SOURCE_DIR}/deps/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/deps/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/deps/imgui)
add_subdirectory(${PROJECT_SOURCE_DIR}/deps/implot)

target_link_libraries(${KONTOMIRE_RENDERER} glm)
target_link_libraries(${KONTOMIRE_RENDERER} glad)

target_link_libraries(${DEMO_APP} glm)
target_link_libraries(${DEMO_APP} glad)
target_link_libraries(${DEMO_APP} glfw)
target_link_libraries(${DEMO_APP} imgui)
target_link_libraries(${DEMO_APP} implot)
target_link_libraries(${DEMO_APP} ${KONTOMIRE_RENDERER})

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