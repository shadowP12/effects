set(MAIN_SRC
	Source/main.cpp
)

set(CORE_SRC
	Source/Core/Scene/CommonTool.cpp
	Source/Core/Scene/RenderScene.cpp
	Source/Core/Utility/FileUtility.cpp
	Source/Core/Gfx/GpuProgram.cpp
)

set(EFFECTS_SRC
	Source/Effects/PBREffect.cpp
)

set(EFFECTS_SRC
	${MAIN_SRC}
	${CORE_SRC}
	${EFFECTS_SRC}
)