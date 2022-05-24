add_library(NuakeUI STATIC 
NuakeUI.cpp Canvas.h Canvas.cpp 
Node.h Node.cpp Style.h 
Renderer.h Renderer.cpp 
FileSystem.h FileSystem.cpp 
Button.h Button.cpp 
Checkbox.h Checkbox.cpp 
InputManager.h Text.h Text.cpp 
StyleSheet.h Font.h Font.cpp
)

target_link_libraries(NuakeUI PUBLIC NuakeRenderer yogacore msdf-atlas-gen)