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
include CMakeFiles/LSM-2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/LSM-2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/LSM-2.dir/flags.make

CMakeFiles/LSM-2.dir/correctness.cc.obj: CMakeFiles/LSM-2.dir/flags.make
CMakeFiles/LSM-2.dir/correctness.cc.obj: ../correctness.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\PROGRAM_CPP\LSM-2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/LSM-2.dir/correctness.cc.obj"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\LSM-2.dir\correctness.cc.obj -c D:\PROGRAM_CPP\LSM-2\correctness.cc

CMakeFiles/LSM-2.dir/correctness.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LSM-2.dir/correctness.cc.i"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\PROGRAM_CPP\LSM-2\correctness.cc > CMakeFiles\LSM-2.dir\correctness.cc.i

CMakeFiles/LSM-2.dir/correctness.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LSM-2.dir/correctness.cc.s"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\PROGRAM_CPP\LSM-2\correctness.cc -o CMakeFiles\LSM-2.dir\correctness.cc.s

CMakeFiles/LSM-2.dir/bloomfilter.cpp.obj: CMakeFiles/LSM-2.dir/flags.make
CMakeFiles/LSM-2.dir/bloomfilter.cpp.obj: ../bloomfilter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\PROGRAM_CPP\LSM-2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/LSM-2.dir/bloomfilter.cpp.obj"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\LSM-2.dir\bloomfilter.cpp.obj -c D:\PROGRAM_CPP\LSM-2\bloomfilter.cpp

CMakeFiles/LSM-2.dir/bloomfilter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LSM-2.dir/bloomfilter.cpp.i"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\PROGRAM_CPP\LSM-2\bloomfilter.cpp > CMakeFiles\LSM-2.dir\bloomfilter.cpp.i

CMakeFiles/LSM-2.dir/bloomfilter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LSM-2.dir/bloomfilter.cpp.s"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\PROGRAM_CPP\LSM-2\bloomfilter.cpp -o CMakeFiles\LSM-2.dir\bloomfilter.cpp.s

CMakeFiles/LSM-2.dir/cache.cpp.obj: CMakeFiles/LSM-2.dir/flags.make
CMakeFiles/LSM-2.dir/cache.cpp.obj: ../cache.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\PROGRAM_CPP\LSM-2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/LSM-2.dir/cache.cpp.obj"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\LSM-2.dir\cache.cpp.obj -c D:\PROGRAM_CPP\LSM-2\cache.cpp

CMakeFiles/LSM-2.dir/cache.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LSM-2.dir/cache.cpp.i"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\PROGRAM_CPP\LSM-2\cache.cpp > CMakeFiles\LSM-2.dir\cache.cpp.i

CMakeFiles/LSM-2.dir/cache.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LSM-2.dir/cache.cpp.s"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\PROGRAM_CPP\LSM-2\cache.cpp -o CMakeFiles\LSM-2.dir\cache.cpp.s

CMakeFiles/LSM-2.dir/kvstore.cc.obj: CMakeFiles/LSM-2.dir/flags.make
CMakeFiles/LSM-2.dir/kvstore.cc.obj: ../kvstore.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\PROGRAM_CPP\LSM-2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/LSM-2.dir/kvstore.cc.obj"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\LSM-2.dir\kvstore.cc.obj -c D:\PROGRAM_CPP\LSM-2\kvstore.cc

CMakeFiles/LSM-2.dir/kvstore.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LSM-2.dir/kvstore.cc.i"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\PROGRAM_CPP\LSM-2\kvstore.cc > CMakeFiles\LSM-2.dir\kvstore.cc.i

CMakeFiles/LSM-2.dir/kvstore.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LSM-2.dir/kvstore.cc.s"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\PROGRAM_CPP\LSM-2\kvstore.cc -o CMakeFiles\LSM-2.dir\kvstore.cc.s

CMakeFiles/LSM-2.dir/memtable.cpp.obj: CMakeFiles/LSM-2.dir/flags.make
CMakeFiles/LSM-2.dir/memtable.cpp.obj: ../memtable.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\PROGRAM_CPP\LSM-2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/LSM-2.dir/memtable.cpp.obj"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\LSM-2.dir\memtable.cpp.obj -c D:\PROGRAM_CPP\LSM-2\memtable.cpp

CMakeFiles/LSM-2.dir/memtable.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LSM-2.dir/memtable.cpp.i"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\PROGRAM_CPP\LSM-2\memtable.cpp > CMakeFiles\LSM-2.dir\memtable.cpp.i

CMakeFiles/LSM-2.dir/memtable.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LSM-2.dir/memtable.cpp.s"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\PROGRAM_CPP\LSM-2\memtable.cpp -o CMakeFiles\LSM-2.dir\memtable.cpp.s

CMakeFiles/LSM-2.dir/sstable.cpp.obj: CMakeFiles/LSM-2.dir/flags.make
CMakeFiles/LSM-2.dir/sstable.cpp.obj: ../sstable.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\PROGRAM_CPP\LSM-2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/LSM-2.dir/sstable.cpp.obj"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\LSM-2.dir\sstable.cpp.obj -c D:\PROGRAM_CPP\LSM-2\sstable.cpp

CMakeFiles/LSM-2.dir/sstable.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LSM-2.dir/sstable.cpp.i"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\PROGRAM_CPP\LSM-2\sstable.cpp > CMakeFiles\LSM-2.dir\sstable.cpp.i

CMakeFiles/LSM-2.dir/sstable.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LSM-2.dir/sstable.cpp.s"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\PROGRAM_CPP\LSM-2\sstable.cpp -o CMakeFiles\LSM-2.dir\sstable.cpp.s

# Object files for target LSM-2
LSM__2_OBJECTS = \
"CMakeFiles/LSM-2.dir/correctness.cc.obj" \
"CMakeFiles/LSM-2.dir/bloomfilter.cpp.obj" \
"CMakeFiles/LSM-2.dir/cache.cpp.obj" \
"CMakeFiles/LSM-2.dir/kvstore.cc.obj" \
"CMakeFiles/LSM-2.dir/memtable.cpp.obj" \
"CMakeFiles/LSM-2.dir/sstable.cpp.obj"

# External object files for target LSM-2
LSM__2_EXTERNAL_OBJECTS =

LSM-2.exe: CMakeFiles/LSM-2.dir/correctness.cc.obj
LSM-2.exe: CMakeFiles/LSM-2.dir/bloomfilter.cpp.obj
LSM-2.exe: CMakeFiles/LSM-2.dir/cache.cpp.obj
LSM-2.exe: CMakeFiles/LSM-2.dir/kvstore.cc.obj
LSM-2.exe: CMakeFiles/LSM-2.dir/memtable.cpp.obj
LSM-2.exe: CMakeFiles/LSM-2.dir/sstable.cpp.obj
LSM-2.exe: CMakeFiles/LSM-2.dir/build.make
LSM-2.exe: CMakeFiles/LSM-2.dir/linklibs.rsp
LSM-2.exe: CMakeFiles/LSM-2.dir/objects1.rsp
LSM-2.exe: CMakeFiles/LSM-2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\PROGRAM_CPP\LSM-2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable LSM-2.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\LSM-2.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/LSM-2.dir/build: LSM-2.exe

.PHONY : CMakeFiles/LSM-2.dir/build

CMakeFiles/LSM-2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\LSM-2.dir\cmake_clean.cmake
.PHONY : CMakeFiles/LSM-2.dir/clean

CMakeFiles/LSM-2.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\PROGRAM_CPP\LSM-2 D:\PROGRAM_CPP\LSM-2 D:\PROGRAM_CPP\LSM-2\cmake-build-debug D:\PROGRAM_CPP\LSM-2\cmake-build-debug D:\PROGRAM_CPP\LSM-2\cmake-build-debug\CMakeFiles\LSM-2.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/LSM-2.dir/depend

