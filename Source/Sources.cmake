set(MAIN_SRC
	Source/main.cpp
)

set(CORE_SRC
	Source/Core/Scene/CommonTool.cpp
	Source/Core/Scene/RenderScene.cpp
	Source/Core/Utility/FileUtility.cpp
)

set(EFFECTS_SRC
	${MAIN_SRC}
	${CORE_SRC}
)