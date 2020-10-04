cmake_minimum_required(VERSION 3.8)


project(maze)


# Grab the screens
file(GLOB_RECURSE screen_sources src/*.cpp src/*.h)

# Executable & linking
add_executable(${PROJECT_NAME} src/main.cpp ${screen_sources})
add_subdirectory(raylib)

if(${PLATFORM} MATCHES "Web")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --shell-file ${CMAKE_SOURCE_DIR}/src/shell.html --preload-file ${CMAKE_SOURCE_DIR}/res@/res")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --shell-file ${CMAKE_SOURCE_DIR}/src/shell.html --preload-file ${CMAKE_SOURCE_DIR}/res@/res")

  set(OUTPUT_EXT ".html")

  # Remove the -rdynamic flag because otherwise emscripten
  # does not generate HTML+JS+WASM files, only a non-working
  # and fat HTML
  string(REPLACE "-rdynamic" "" CMAKE_SHARED_LIBRARY_LINK_C_FLAGS ${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS})

  set_target_properties(
      ${PROJECT_NAME}
      PROPERTIES
          OUTPUT_NAME "maze"
          SUFFIX ".html"
  )

endif()

target_link_libraries(${PROJECT_NAME} raylib glfw)
