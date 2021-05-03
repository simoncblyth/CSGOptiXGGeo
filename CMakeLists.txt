cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
set(name CSGOptiXGGeo)
project(${name} VERSION 0.1.0)
include(OpticksBuildOptions)

find_package(CUDA REQUIRED)   
#find_package(CSG REQUIRED)
#find_package(OptiX REQUIRED)   
find_package(GGeo REQUIRED CONFIG)
find_package(CSGOptiX REQUIRED CONFIG)

message(STATUS "CSG_FOUND      : ${CSG_FOUND}      CSG_INCLUDE_DIRS      : ${CSG_INCLUDE_DIRS}")
message(STATUS "OptiX_FOUND    : ${OptiX_FOUND}    OptiX_INCLUDE_DIRS    : ${OptiX_INCLUDE_DIRS}")
message(STATUS "GGeo_FOUND     : ${GGeo_FOUND}     GGeo_INCLUDE_DIRS     : ${GGeo_INCLUDE_DIRS}")
message(STATUS "CSGOptiX_FOUND : ${CSGOptiX_FOUND} CSGOptiX_INCLUDE_DIRS : ${CSGOptiX_INCLUDE_DIRS}")

set(SOURCES)

list(APPEND SOURCES
    Converter.cc 
    ${name}.cc
)

include_directories(
    ${CUDA_INCLUDE_DIRS}
    ${OptiX_INCLUDE}
    ${CSGOptiX_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR} 
)

add_executable(${name} ${SOURCES})

target_link_libraries(${name} PUBLIC
    Opticks::GGeo 
    Opticks::CSGOptiX 
)

install(TARGETS ${name}  DESTINATION lib)

