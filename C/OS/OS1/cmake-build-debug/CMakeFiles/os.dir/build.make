# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

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
CMAKE_COMMAND = /cygdrive/c/Users/idan/AppData/Local/JetBrains/CLion2020.2/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/idan/AppData/Local/JetBrains/CLion2020.2/cygwin_cmake/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/d/Users/idan/CLionProjects/OS1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/d/Users/idan/CLionProjects/OS1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/os.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/os.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/os.dir/flags.make

CMakeFiles/os.dir/os.c.o: CMakeFiles/os.dir/flags.make
CMakeFiles/os.dir/os.c.o: ../os.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/d/Users/idan/CLionProjects/OS1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/os.dir/os.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/os.dir/os.c.o   -c /cygdrive/d/Users/idan/CLionProjects/OS1/os.c

CMakeFiles/os.dir/os.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/os.dir/os.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/d/Users/idan/CLionProjects/OS1/os.c > CMakeFiles/os.dir/os.c.i

CMakeFiles/os.dir/os.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/os.dir/os.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/d/Users/idan/CLionProjects/OS1/os.c -o CMakeFiles/os.dir/os.c.s

# Object files for target os
os_OBJECTS = \
"CMakeFiles/os.dir/os.c.o"

# External object files for target os
os_EXTERNAL_OBJECTS =

os.exe: CMakeFiles/os.dir/os.c.o
os.exe: CMakeFiles/os.dir/build.make
os.exe: CMakeFiles/os.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/d/Users/idan/CLionProjects/OS1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable os.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/os.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/os.dir/build: os.exe

.PHONY : CMakeFiles/os.dir/build

CMakeFiles/os.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/os.dir/cmake_clean.cmake
.PHONY : CMakeFiles/os.dir/clean

CMakeFiles/os.dir/depend:
	cd /cygdrive/d/Users/idan/CLionProjects/OS1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/d/Users/idan/CLionProjects/OS1 /cygdrive/d/Users/idan/CLionProjects/OS1 /cygdrive/d/Users/idan/CLionProjects/OS1/cmake-build-debug /cygdrive/d/Users/idan/CLionProjects/OS1/cmake-build-debug /cygdrive/d/Users/idan/CLionProjects/OS1/cmake-build-debug/CMakeFiles/os.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/os.dir/depend

