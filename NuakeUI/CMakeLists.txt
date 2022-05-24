add_library(NuakeUI STATIC 
NuakeUI.cpp Canvas.h Canvas.cpp 
Node.h Node.cpp Style.h 
Renderer.h Renderer.cpp 
FileSystem.h FileSystem.cpp 
Button.h Button.cpp 
Checkbox.h Checkbox.cpp 
InputManager.h Text.h Text.cpp 
StyleSheet.h Font.h Font.cpp
FontLoader.h 
FontManager.h FontManager.cpp 
Inspector.h StyleSheet.h StyleSheet.cpp 
CanvasParser.h CanvasParser.cpp 
Vendors/katana-parser/foundation.h Vendors/katana-parser/foundation.c
Vendors/katana-parser/katana.h Vendors/katana-parser/katana.lex.h Vendors/katana-parser/katana.lex.c
Vendors/katana-parser/katana.tab.h Vendors/katana-parser/katana.tab.c 
Vendors/katana-parser/parser.h Vendors/katana-parser/parser.c Vendors/katana-parser/selector.h 
Vendors/katana-parser/selector.c Vendors/katana-parser/tokenizer.h Vendors/katana-parser/tokenizer.c 
StyleSheetParser.h StyleSheetParser.cpp 
)

target_link_libraries(NuakeUI PUBLIC NuakeRenderer yogacore msdf-atlas-gen)