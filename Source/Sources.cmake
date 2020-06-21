set(MAIN_SRC
	Source/main.cpp
)

set(CORE_SRC
	Source/Core/Datas/MeshData.cpp
	Source/Core/Datas/PixelData.cpp
	Source/Core/Scene/CommonTool.cpp
	Source/Core/Scene/RenderScene.cpp
	Source/Core/Scene/Node.cpp
	Source/Core/Scene/Scene.cpp
	Source/Core/Scene/Component.cpp
	Source/Core/Utility/FileUtility.cpp
	Source/Core/Gfx/GfxDebug.cpp
	Source/Core/Gfx/GfxResources.cpp
	Source/Core/RenderResources/Mesh.cpp
	Source/Core/RenderResources/Material.cpp
	Source/Core/RenderResources/PBRMaterial.cpp
	Source/Core/Renderer/Renderable.cpp
	Source/Core/Renderer/RenderView.cpp
	Source/Core/Renderer/Renderer.cpp
	Source/Core/Renderer/RenderHelper.cpp
	Source/Core/Renderer/IBLUtility.cpp
	Source/Core/Components/CRenderable.cpp
	Source/Core/Components/CCamera.cpp
)

set(EFFECTS_SRC
	Source/Effects/PBREffect.cpp
	Source/Effects/DebugEffect.cpp
)

set(UI_SRC
	Source/UI/IMGUI/imgui.cpp
	Source/UI/IMGUI/imgui_demo.cpp
	Source/UI/IMGUI/imgui_draw.cpp
	Source/UI/IMGUI/imgui_widgets.cpp
	Source/UI/IMGUI/imgui_impl_glfw.cpp
	Source/UI/IMGUI/imgui_impl_opengl3.cpp
	Source/UI/UISystem.cpp
	Source/UI/LightWidget.cpp
)

set(MATH_SRC
	Source/Math/Geometry.cpp
)

set(IMPORTERS_SRC
	Source/Importers/GltfImporter.cpp
	Source/Importers/TextureImporter.cpp
)

set(EFFECTS_SRC
	${MAIN_SRC}
	${CORE_SRC}
	${EFFECTS_SRC}
	${UI_SRC}
	${MATH_SRC}
	${IMPORTERS_SRC}
)