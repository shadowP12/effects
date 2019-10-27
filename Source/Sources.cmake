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

set(UI_SRC
	Source/UI/IMGUI/imgui.cpp
	Source/UI/IMGUI/imgui_draw.cpp
	Source/UI/IMGUI/imgui_widgets.cpp
	Source/UI/IMGUI/imgui_impl_glfw.cpp
	Source/UI/IMGUI/imgui_impl_opengl3.cpp
	Source/UI/UISystem.cpp
	Source/UI/LightWidget.cpp
)

set(EFFECTS_SRC
	${MAIN_SRC}
	${CORE_SRC}
	${EFFECTS_SRC}
	${UI_SRC}
)