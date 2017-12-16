cmake_minimum_required(VERSION 3.8)
project(Boids_Algorithm_OpenCL)

set(CMAKE_CXX_STANDARD 11)


find_package(glfw3 3.2.1 REQUIRED)
find_package (OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS} -L/usr/local/lib -framework Cocoa -framework IOKit -framework OpenCL -framework CoreVideo -lglfw3")

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /usr/local/lib)
link_directories(/usr/local/lib)


list(APPEND CMAKE_MODULE_PATH "/Users/Np/Documents/FindTBB-master")
find_package(TBB COMPONENTS tbbmalloc tbbmalloc_proxy tbb_preview)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -framework OpenGL")

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

#list(APPEND CMAKE_MODULE_PATH "/Users/Np/Documents/CMake-master/Modules/FindOpenCL.cmake")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(OpenCL REQUIRED)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS} -L/usr/local/lib -framework OpenCL")

message(STATUS "OpenCL found: ${OPENCL_FOUND}")
message(STATUS "OpenCL includes: ${OPENCL_INCLUDE_DIRS}")
message(STATUS "OpenCL CXX includes: ${OPENCL_HAS_CPP_BINDINGS}")
message(STATUS "OpenCL libraries: ${OPENCL_LIBRARIES}")






set(SOURCE_FILES main.cpp Boid.cpp Boid.h Flock.cpp Flock.h Engine.cpp Engine.h Draw.cpp Draw.h FlockPool.cpp FlockPool.h MegaFlock.cpp MegaFlock.h)
add_executable(Boids_Algorithm_OpenCL ${SOURCE_FILES})




target_link_libraries(Boids_Algorithm_OpenCL tbb)
target_link_libraries(Boids_Algorithm_OpenCL OpenCL::OpenCL)
