# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/projects/fball_project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/projects/fball_project/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/fball.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fball.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fball.dir/flags.make

CMakeFiles/fball.dir/main.cpp.o: CMakeFiles/fball.dir/flags.make
CMakeFiles/fball.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/projects/fball_project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fball.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fball.dir/main.cpp.o -c /home/pi/projects/fball_project/main.cpp

CMakeFiles/fball.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fball.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/projects/fball_project/main.cpp > CMakeFiles/fball.dir/main.cpp.i

CMakeFiles/fball.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fball.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/projects/fball_project/main.cpp -o CMakeFiles/fball.dir/main.cpp.s

# Object files for target fball
fball_OBJECTS = \
"CMakeFiles/fball.dir/main.cpp.o"

# External object files for target fball
fball_EXTERNAL_OBJECTS =

fball: CMakeFiles/fball.dir/main.cpp.o
fball: CMakeFiles/fball.dir/build.make
fball: CMakeFiles/fball.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/projects/fball_project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable fball"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fball.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fball.dir/build: fball

.PHONY : CMakeFiles/fball.dir/build

CMakeFiles/fball.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fball.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fball.dir/clean

CMakeFiles/fball.dir/depend:
	cd /home/pi/projects/fball_project/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/projects/fball_project /home/pi/projects/fball_project /home/pi/projects/fball_project/cmake-build-debug /home/pi/projects/fball_project/cmake-build-debug /home/pi/projects/fball_project/cmake-build-debug/CMakeFiles/fball.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fball.dir/depend

