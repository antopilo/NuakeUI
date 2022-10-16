cmake_minimum_required(VERSION 3.6)

project(NuakeUI)
set (CMAKE_CXX_STANDARD 17)
set (CXX_STANDARD_REQUIRED ON)

add_subdirectory(NuakeUI/)

add_subdirectory(./Dependencies/NuakeRenderer)
include_directories(./Dependencies/NuakeRenderer/)

add_subdirectory(./Dependencies/yoga_cpp)
include_directories(./Dependencies/yoga_cpp/)

add_subdirectory(./Dependencies/msdf-atlas-gen)
include_directories(./Dependencies/msdf-atlas-gen)
include_directories(./Dependencies/msdf-atlas-gen/msdf-atlas-gen)

add_subdirectory(./Dependencies/nanosvg)
include_directories(./Dependencies/nanosvg/src) 




