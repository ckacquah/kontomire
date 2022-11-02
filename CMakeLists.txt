cmake_minimum_required(VERSION 3.10)

set(KONTOMIRE_RENDERER "kontomire")
set(KONTOMIRE_PLAYGROUND "kontomire-playground")

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

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wconversion")
endif()

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

message(STATUS "Started CMake for ${KONTOMIRE_PLAYGROUND} v${PROJECT_VERSION}...\n")

# Project target
add_library(
  ${KONTOMIRE_RENDERER}
  ${PROJECT_SOURCE_DIR}/src/kontomire/core/buffer.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/core/shader.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/core/texture.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/core/renderAPI.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/core/renderer.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/core/renderer2d.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/core/framebuffer.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/core/vertex_arrays.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/platform/opengl/buffer.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/platform/opengl/shader.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/platform/opengl/texture.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/platform/opengl/renderAPI.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/platform/opengl/framebuffer.cc
  ${PROJECT_SOURCE_DIR}/src/kontomire/platform/opengl/vertex_arrays.cc
)

target_include_directories(${KONTOMIRE_RENDERER} PUBLIC ${PROJECT_SOURCE_DIR}/deps)
target_include_directories(${KONTOMIRE_RENDERER} PUBLIC ${PROJECT_SOURCE_DIR}/deps/stb)
target_include_directories(${KONTOMIRE_RENDERER} PUBLIC ${PROJECT_SOURCE_DIR}/include)

# The demo application
add_executable(
  ${KONTOMIRE_PLAYGROUND}
  ${PROJECT_SOURCE_DIR}/src/playground/main.cc
  ${PROJECT_SOURCE_DIR}/src/playground/window.cc
  ${PROJECT_SOURCE_DIR}/src/playground/application.cc
  ${PROJECT_SOURCE_DIR}/src/playground/imgui_manager.cc
)

target_include_directories(${KONTOMIRE_PLAYGROUND} PUBLIC ${PROJECT_SOURCE_DIR}/deps)
target_include_directories(${KONTOMIRE_PLAYGROUND} PUBLIC ${PROJECT_SOURCE_DIR}/include)

configure_file(${PROJECT_SOURCE_DIR}/assets/config/imgui.ini assets/config/desktop.ini COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/assets/textures/Sword.png assets/textures/sword.png COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/assets/textures/texture.jpg assets/textures/texture.jpg COPYONLY)

configure_file(${PROJECT_SOURCE_DIR}/assets/shaders/flat-color.glsl assets/shaders/flat-color.glsl COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/assets/shaders/basic-texture.glsl assets/shaders/basic-texture.glsl COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/assets/shaders/render2d/quad.glsl assets/shaders/render2d/quad.glsl COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/assets/shaders/render2d/line.glsl assets/shaders/render2d/line.glsl COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/assets/shaders/render2d/circle.glsl assets/shaders/render2d/circle.glsl COPYONLY)

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

target_link_libraries(${KONTOMIRE_PLAYGROUND} glm)
target_link_libraries(${KONTOMIRE_PLAYGROUND} glad)
target_link_libraries(${KONTOMIRE_PLAYGROUND} glfw)
target_link_libraries(${KONTOMIRE_PLAYGROUND} imgui)
target_link_libraries(${KONTOMIRE_PLAYGROUND} implot)
target_link_libraries(${KONTOMIRE_PLAYGROUND} ${KONTOMIRE_RENDERER})

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