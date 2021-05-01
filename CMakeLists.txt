cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
set(name CSGOptiXGGeo)
project(${name} VERSION 0.1.0)
include(OpticksBuildOptions)

find_package(CUDA REQUIRED)   
find_package(CSG REQUIRED)
find_package(OptiX REQUIRED)   


message(STATUS "CSG_FOUND : ${CSG_FOUND} ")


include_directories(
    ${CUDA_INCLUDE_DIRS}
    ${OptiX_INCLUDE}
    ${CMAKE_CURRENT_SOURCE_DIR} 
)

find_package(GGeo REQUIRED CONFIG)

add_executable(${name} ${name}.cc)
target_link_libraries(${name} Opticks::GGeo CSG )
install(TARGETS ${name}  DESTINATION lib)

