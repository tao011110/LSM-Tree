# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.19

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

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\PROGRAM_CPP\LSM-2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\PROGRAM_CPP\LSM-2\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/LSM_2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/LSM_2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/LSM_2.dir/flags.make

CMakeFiles/LSM_2.dir/main.cpp.obj: CMakeFiles/LSM_2.dir/flags.make
CMakeFiles/LSM_2.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\PROGRAM_CPP\LSM-2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/LSM_2.dir/main.cpp.obj"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\LSM_2.dir\main.cpp.obj -c D:\PROGRAM_CPP\LSM-2\main.cpp

CMakeFiles/LSM_2.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LSM_2.dir/main.cpp.i"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\PROGRAM_CPP\LSM-2\main.cpp > CMakeFiles\LSM_2.dir\main.cpp.i

CMakeFiles/LSM_2.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LSM_2.dir/main.cpp.s"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\PROGRAM_CPP\LSM-2\main.cpp -o CMakeFiles\LSM_2.dir\main.cpp.s

# Object files for target LSM_2
LSM_2_OBJECTS = \
"CMakeFiles/LSM_2.dir/main.cpp.obj"

# External object files for target LSM_2
LSM_2_EXTERNAL_OBJECTS =

LSM_2.exe: CMakeFiles/LSM_2.dir/main.cpp.obj
LSM_2.exe: CMakeFiles/LSM_2.dir/build.make
LSM_2.exe: CMakeFiles/LSM_2.dir/linklibs.rsp
LSM_2.exe: CMakeFiles/LSM_2.dir/objects1.rsp
LSM_2.exe: CMakeFiles/LSM_2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\PROGRAM_CPP\LSM-2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable LSM_2.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\LSM_2.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/LSM_2.dir/build: LSM_2.exe

.PHONY : CMakeFiles/LSM_2.dir/build

CMakeFiles/LSM_2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\LSM_2.dir\cmake_clean.cmake
.PHONY : CMakeFiles/LSM_2.dir/clean

CMakeFiles/LSM_2.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\PROGRAM_CPP\LSM-2 D:\PROGRAM_CPP\LSM-2 D:\PROGRAM_CPP\LSM-2\cmake-build-debug D:\PROGRAM_CPP\LSM-2\cmake-build-debug D:\PROGRAM_CPP\LSM-2\cmake-build-debug\CMakeFiles\LSM_2.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/LSM_2.dir/depend

