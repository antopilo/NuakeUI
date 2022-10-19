set(HEADERS 
	Nodes/Canvas.h 
	Nodes/Node.h  
	Styles/Style.h 
	Renderer.h 
	FileSystem.h  
	Nodes/Button.h 
	InputManager.h 
	Nodes/Text.h  
	Nodes/TextInput.h  
	Styles/StyleSheet.h 
	Font/Font.h 
	Font/FontLoader.h 
	Font/FontManager.h 
	Inspector.h 
	Styles/StyleSheet.h 
	Parsers/CanvasParser.h 
	Parsers/StyleSheetParser.h 
	InputManager.h 
	StringHelper.h 
	Nodes/NodeState.h 
	DataBinding/DataBindObject.h 
	DataBinding/DataBindOperation.h 
	DataBinding/DataModelOperations.h 
	NuakeUI.h 
	Nodes/NodeStyle.h 
	IController.h 
)

set(SOURCES 
	NuakeUI.cpp 
	Nodes/Canvas.cpp 
	Nodes/Node.cpp 
	Renderer.cpp 
	FileSystem.cpp 
	Nodes/Text.cpp 
	Font/FontManager.cpp 
	Styles/StyleSheet.cpp 
	Parsers/CanvasParser.cpp 
	Font/Font.cpp 
	Nodes/Button.cpp 
	Parsers/StyleSheetParser.cpp 
	InputManager.cpp 
	DataBinding/DataBindObject.cpp  
	DataBinding/DataModelOperations.cpp 
	StringHelper.cpp 
)

set(VENDOR_SOURCES 
	Vendors/katana-parser/foundation.h 
	Vendors/katana-parser/foundation.c 
	Vendors/katana-parser/katana.lex.c
	Vendors/katana-parser/katana.tab.c 
	Vendors/katana-parser/parser.c 
	Vendors/katana-parser/selector.c 
	Vendors/katana-parser/tokenizer.c )

set(VENDOR_HEADERS 
	Vendors/katana-parser/katana.h 
	Vendors/katana-parser/katana.lex.h 
	Vendors/katana-parser/katana.tab.h 
	Vendors/katana-parser/parser.h 
	Vendors/katana-parser/selector.h 
	Vendors/katana-parser/tokenizer.h
)

add_library(NuakeUI STATIC ${HEADERS} ${SOURCES} ${VENDOR_HEADERS} ${VENDOR_SOURCES})

target_link_libraries(NuakeUI PUBLIC 
	NuakeRenderer 
	yogacore 
	msdf-atlas-gen 
	nanosvg
	nanosvgrast
)

target_include_directories(NuakeUI PUBLIC ../Dependencies/nanosvg/src)