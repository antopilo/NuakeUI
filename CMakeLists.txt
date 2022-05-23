cmake_minimum_required(VERSION 3.6)

project(NuakeUI)
set (CMAKE_CXX_STANDARD 17)
set (CXX_STANDARD_REQUIRED ON)

add_subdirectory(./Dependencies/NuakeRenderer)
include_directories(./Dependencies/NuakeRenderer/)

add_subdirectory(./Dependencies/yoga_cpp)
include_directories(./Dependencies/yoga_cpp/)


add_library(NuakeUI STATIC 
NuakeUI/NuakeUI.cpp NuakeUI/Canvas.h NuakeUI/Canvas.cpp 
NuakeUI/Node.h NuakeUI/Node.cpp NuakeUI/Style.h 
NuakeUI/Renderer.h NuakeUI/Renderer.cpp 
NuakeUI/FileSystem.h NuakeUI/FileSystem.cpp 
NuakeUI/Button.h NuakeUI/Button.cpp
)

target_link_libraries(NuakeUI PUBLIC NuakeRenderer yogacore)
