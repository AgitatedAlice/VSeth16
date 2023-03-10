cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(rayvcgafc)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 4.2.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

# Our Project

file(GLOB SOURCES CONFIGURE_DEPENDS
	"./src/*.h"
	"./src/*.cpp"
	"./src/*.c"
	"/extras/*.h"
)

add_executable(${PROJECT_NAME} ${SOURCES})

#add_executable(${PROJECT_NAME} core_basic_window.c)
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()



###Debug
add_compile_options("-g")

### Testing
enable_testing()

macro(addtest TESTNAME)

	      add_executable(${TESTNAME} "test/${TESTNAME}.c")
	      add_test(NAME ${TESTNAME} COMMAND $<TARGET_FILE:${TESTNAME}>)
endmacro()

addtest(testPcMovesToSecondInstructionThenHalts)
addtest(testHaltHaltsVM)
addtest(testLIWInA)
addtest(testLIWInD)
addtest(testLIWSkipsPC)
addtest(testLIBInD)
addtest(testLIBInA)
addtest(testPSHFromAAndPOPInD)
addtest(testPSHFromDAndPOPInA)
addtest(testPSHIncreasesStackPointer)
addtest(testPOPDecreasesStackPointer)
addtest(testPOPEmptyStack)
addtest(testPSHFullStack)
addtest(testPOPFullStack)
addTest(testPOPEmptyStackPopsLastValue)
addTest(testPSHFullStackOverridesLastValue)


file(INSTALL ./extra/boot.hex DESTINATION .)
