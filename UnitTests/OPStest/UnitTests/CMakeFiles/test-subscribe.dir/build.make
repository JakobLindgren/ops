# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.1

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /sw/tools/cmake/cmake-3.1.0-rc1-Linux-i386/bin/cmake

# The command to remove a file.
RM = /sw/tools/cmake/cmake-3.1.0-rc1-Linux-i386/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pane/OPS4/ops4/UnitTests/OPStest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pane/OPS4/ops4/UnitTests/OPStest

# Include any dependencies generated for this target.
include UnitTests/CMakeFiles/test-subscribe.dir/depend.make

# Include the progress variables for this target.
include UnitTests/CMakeFiles/test-subscribe.dir/progress.make

# Include the compile flags for this target's objects.
include UnitTests/CMakeFiles/test-subscribe.dir/flags.make

UnitTests/CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.o: UnitTests/CMakeFiles/test-subscribe.dir/flags.make
UnitTests/CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.o: UnitTests/SubscribeDataAndTest.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pane/OPS4/ops4/UnitTests/OPStest/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object UnitTests/CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.o"
	cd /home/pane/OPS4/ops4/UnitTests/OPStest/UnitTests && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.o -c /home/pane/OPS4/ops4/UnitTests/OPStest/UnitTests/SubscribeDataAndTest.cpp

UnitTests/CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.i"
	cd /home/pane/OPS4/ops4/UnitTests/OPStest/UnitTests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pane/OPS4/ops4/UnitTests/OPStest/UnitTests/SubscribeDataAndTest.cpp > CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.i

UnitTests/CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.s"
	cd /home/pane/OPS4/ops4/UnitTests/OPStest/UnitTests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pane/OPS4/ops4/UnitTests/OPStest/UnitTests/SubscribeDataAndTest.cpp -o CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.s

UnitTests/CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.o.requires:
.PHONY : UnitTests/CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.o.requires

UnitTests/CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.o.provides: UnitTests/CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.o.requires
	$(MAKE) -f UnitTests/CMakeFiles/test-subscribe.dir/build.make UnitTests/CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.o.provides.build
.PHONY : UnitTests/CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.o.provides

UnitTests/CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.o.provides.build: UnitTests/CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.o

# Object files for target test-subscribe
test__subscribe_OBJECTS = \
"CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.o"

# External object files for target test-subscribe
test__subscribe_EXTERNAL_OBJECTS =

UnitTests/test-subscribe: UnitTests/CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.o
UnitTests/test-subscribe: UnitTests/CMakeFiles/test-subscribe.dir/build.make
UnitTests/test-subscribe: /sw/tools/google-test/1.7/google-build/googlemock/gtest/libgtest.a
UnitTests/test-subscribe: /home/pane/OPS4/ops4/Cpp/lib64/libOPSopt.a
UnitTests/test-subscribe: UnitTests/CMakeFiles/test-subscribe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable test-subscribe"
	cd /home/pane/OPS4/ops4/UnitTests/OPStest/UnitTests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test-subscribe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
UnitTests/CMakeFiles/test-subscribe.dir/build: UnitTests/test-subscribe
.PHONY : UnitTests/CMakeFiles/test-subscribe.dir/build

UnitTests/CMakeFiles/test-subscribe.dir/requires: UnitTests/CMakeFiles/test-subscribe.dir/SubscribeDataAndTest.cpp.o.requires
.PHONY : UnitTests/CMakeFiles/test-subscribe.dir/requires

UnitTests/CMakeFiles/test-subscribe.dir/clean:
	cd /home/pane/OPS4/ops4/UnitTests/OPStest/UnitTests && $(CMAKE_COMMAND) -P CMakeFiles/test-subscribe.dir/cmake_clean.cmake
.PHONY : UnitTests/CMakeFiles/test-subscribe.dir/clean

UnitTests/CMakeFiles/test-subscribe.dir/depend:
	cd /home/pane/OPS4/ops4/UnitTests/OPStest && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pane/OPS4/ops4/UnitTests/OPStest /home/pane/OPS4/ops4/UnitTests/OPStest/UnitTests /home/pane/OPS4/ops4/UnitTests/OPStest /home/pane/OPS4/ops4/UnitTests/OPStest/UnitTests /home/pane/OPS4/ops4/UnitTests/OPStest/UnitTests/CMakeFiles/test-subscribe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : UnitTests/CMakeFiles/test-subscribe.dir/depend

