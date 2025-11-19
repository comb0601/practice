#!/usr/bin/env python3
"""
Generate complete Visual Studio solution for C++ Tutorial Course
Includes all 370+ lessons with proper organization
"""

import os
import json

# Part 1: C++ Fundamentals - 80 lessons
part1_lessons = [
    "Lesson01_HelloWorld", "Lesson02_Variables", "Lesson03_Operators", "Lesson04_IfElse",
    "Lesson05_Switch", "Lesson06_WhileLoop", "Lesson07_ForLoop", "Lesson08_BreakContinue",
    "Lesson09_NestedLoops", "Lesson10_Patterns", "Lesson11_Functions", "Lesson12_Parameters",
    "Lesson13_Overloading", "Lesson14_Recursion", "Lesson15_DefaultArgs", "Lesson16_Arrays",
    "Lesson17_2DArrays", "Lesson18_ArrayAlgorithms", "Lesson19_CStrings", "Lesson20_StdString",
    "Lesson21_Pointers", "Lesson22_PointerArithmetic", "Lesson23_PointersArrays",
    "Lesson24_PointersFunctions", "Lesson25_DynamicMemory", "Lesson26_References",
    "Lesson27_ConstPointers", "Lesson28_PointerVsReference", "Lesson29_NullPointers",
    "Lesson30_SmartPointers", "Lesson31_Classes", "Lesson32_Constructors",
    "Lesson33_MemberFunctions", "Lesson34_AccessSpecifiers", "Lesson35_Encapsulation",
    "Lesson36_StaticMembers", "Lesson37_FriendFunctions", "Lesson38_OperatorOverload1",
    "Lesson39_OperatorOverload2", "Lesson40_CopyConstructor", "Lesson41_Inheritance",
    "Lesson42_InheritanceTypes", "Lesson43_MultipleInheritance", "Lesson44_Polymorphism",
    "Lesson45_AbstractClasses", "Lesson46_FunctionTemplates", "Lesson47_ClassTemplates",
    "Lesson48_TemplateSpecialization", "Lesson49_VariadicTemplates", "Lesson50_STLOverview",
    "Lesson51_Vector", "Lesson52_List", "Lesson53_Stack", "Lesson54_Set", "Lesson55_Map",
    "Lesson56_UnorderedMap", "Lesson57_Algorithms1", "Lesson58_Algorithms2", "Lesson59_Iterators",
    "Lesson60_Lambdas", "Lesson61_Exceptions", "Lesson62_CustomExceptions",
    "Lesson63_ExceptionSafety", "Lesson64_FileIO", "Lesson65_BinaryFileIO",
    "Lesson66_FileStreams", "Lesson67_StringStreams", "Lesson68_ModernCPP",
    "Lesson69_MoveSemantics", "Lesson70_SmartPointersAdvanced", "Lesson71_Threads",
    "Lesson72_ThreadSync", "Lesson73_Mutex", "Lesson74_ConditionVariables",
    "Lesson75_AsyncFuture", "Lesson76_Namespaces", "Lesson77_Preprocessor",
    "Lesson78_Compilation", "Lesson79_Debugging", "Lesson80_BestPractices"
]

# Part 2: MFC UI Development - 90 lessons
part2_lessons = [
    ("Lesson01_SimpleMFC", "SimpleMFC"),
    ("Lesson02_Calculator", "Calculator"),
    ("Lesson03_MessageBoxes", "MessageBoxes"),
    ("Lesson04_MessageMaps", "MessageMaps"),
    ("Lesson05_WizardGenerated", "WizardGenerated"),
    ("Lesson06_DialogLifecycle", "DialogLifecycle"),
    ("Lesson07_BasicControls", "BasicControls"),
    ("Lesson08_ListComboControls", "ListComboControls"),
    ("Lesson09_DDXDemo", "DDXDemo"),
    ("Lesson10_ResourceEditor", "ResourceEditor"),
    ("Lesson11_EventHandling", "EventHandling"),
    ("Lesson12_CommonDialogs", "CommonDialogs"),
    ("Lesson13_MultipleDialogs", "MultipleDialogs"),
    ("Lesson14_CustomDialogs", "CustomDialogs"),
    ("Lesson15_DebuggingDemo", "DebuggingDemo"),
    ("Lesson16_DocViewIntro", "DocViewIntro"),
    ("Lesson17_SDIApplication", "SDIApplication"),
    ("Lesson18_DocumentDemo", "DocumentDemo"),
    ("Lesson19_ViewDemo", "ViewDemo"),
    ("Lesson20_Serialization", "Serialization"),
    ("Lesson21_AdvancedSerialization", "AdvancedSerialization"),
    ("Lesson22_MultipleViews", "MultipleViews"),
    ("Lesson23_SplitterWindow", "SplitterWindow"),
    ("Lesson24_MDIApplication", "MDIApplication"),
    ("Lesson25_FrameWindows", "FrameWindows"),
    ("Lesson26_CommandRouting", "CommandRouting"),
    ("Lesson27_MenusToolbars", "MenusToolbars"),
    ("Lesson28_StatusBar", "StatusBar"),
    ("Lesson29_DockingToolbars", "DockingToolbars"),
    ("Lesson30_CompleteSDI", "CompleteSDI"),
    ("Lesson31_ListControlReport", "ListControlReport"),
    ("Lesson32_ListControlIcons", "ListControlIcons"),
    ("Lesson33_TreeControl", "TreeControl"),
    ("Lesson34_RichEdit", "RichEdit"),
    ("Lesson35_ProgressSlider", "ProgressSlider"),
    ("Lesson36_TabControl", "TabControl"),
    ("Lesson37_DateTimePicker", "DateTimePicker"),
    ("Lesson38_IPAddressControl", "IPAddressControl"),
    ("Lesson39_PropertySheet", "PropertySheet"),
    ("Lesson40_Wizard", "Wizard"),
    ("Lesson41_Subclassing", "Subclassing"),
    ("Lesson42_OwnerDraw", "OwnerDraw"),
    ("Lesson43_Notifications", "Notifications"),
    ("Lesson44_DynamicControls", "DynamicControls"),
    ("Lesson45_AllControls", "AllControls"),
    ("Lesson46_GDIIntro", "GDIIntro"),
    ("Lesson47_DeviceContexts", "DeviceContexts"),
    ("Lesson48_PensBrushes", "PensBrushes"),
    ("Lesson49_DrawingShapes", "DrawingShapes"),
    ("Lesson50_TextDrawing", "TextDrawing"),
    ("Lesson51_Colors", "Colors"),
    ("Lesson52_Bitmaps", "Bitmaps"),
    ("Lesson53_DoubleBuffering", "DoubleBuffering"),
    ("Lesson54_Coordinates", "Coordinates"),
    ("Lesson55_PrintingBasic", "PrintingBasic"),
    ("Lesson56_PrintPreview", "PrintPreview"),
    ("Lesson57_AdvancedPrinting", "AdvancedPrinting"),
    ("Lesson58_CustomDrawing", "CustomDrawing"),
    ("Lesson59_MouseDrawing", "MouseDrawing"),
    ("Lesson60_DrawingApp", "DrawingApp"),
    ("Lesson61_Threading", "Threading"),
    ("Lesson62_WorkerThreads", "WorkerThreads"),
    ("Lesson63_ThreadSync", "ThreadSync"),
    ("Lesson64_RegularDLL", "RegularDLL"),
    ("Lesson65_ExtensionDLL", "ExtensionDLL"),
    ("Lesson66_ActiveX", "ActiveX"),
    ("Lesson67_COM", "COM"),
    ("Lesson68_ODBC", "ODBC"),
    ("Lesson69_ADO", "ADO"),
    ("Lesson70_Sockets", "Sockets"),
    ("Lesson71_WinInet", "WinInet"),
    ("Lesson72_Registry", "Registry"),
    ("Lesson73_INIFiles", "INIFiles"),
    ("Lesson74_Clipboard", "Clipboard"),
    ("Lesson75_DragDrop", "DragDrop"),
    ("Lesson76_Architecture", "Architecture"),
    ("Lesson77_TextEditor", "TextEditor"),
    ("Lesson78_ImageViewer", "ImageViewer"),
    ("Lesson79_FileManager", "FileManager"),
    ("Lesson80_DatabaseApp", "DatabaseApp"),
    ("Lesson81_DrawingProgram", "DrawingProgram"),
    ("Lesson82_ChatApp", "ChatApp"),
    ("Lesson83_Optimization", "Optimization"),
    ("Lesson84_SecurityApp", "SecurityApp"),
    ("Lesson85_ReportGenerator", "ReportGenerator"),
    ("Lesson86_GameEngine", "GameEngine"),
    ("Lesson87_MediaPlayer", "MediaPlayer"),
    ("Lesson88_WebBrowser", "WebBrowser"),
    ("Lesson89_PluginSystem", "PluginSystem"),
    ("Lesson90_FinalProject", "FinalProject")
]

# Part 3: 3D Rendering - 100 lessons (organized by category)
part3_lessons = {
    "Lessons01-20_3DMath": [
        ("Lesson01_3DMathIntro", "Lesson01"), ("Lesson02_VectorBasics", "Lesson02"),
        ("Lesson03_VectorMath", "Lesson03"), ("Lesson04_DotCrossProduct", "Lesson04"),
        ("Lesson05_MatrixIntro", "Lesson05"), ("Lesson06_MatrixOperations", "Lesson06"),
        ("Lesson07_Transformations", "Lesson07"), ("Lesson08_MatrixMultiplication", "Lesson08"),
        ("Lesson09_CoordinateSystems", "Lesson09"), ("Lesson10_HomogeneousCoords", "Lesson10"),
        ("Lesson11_QuaternionIntro", "Lesson11"), ("Lesson12_QuaternionRotations", "Lesson12"),
        ("Lesson13_CameraMath", "Lesson13"), ("Lesson14_ViewMatrix", "Lesson14"),
        ("Lesson15_ProjectionMatrices", "Lesson15"), ("Lesson16_ViewportTransform", "Lesson16"),
        ("Lesson17_FrustumCulling", "Lesson17"), ("Lesson18_RayIntersection", "Lesson18"),
        ("Lesson19_BoundingVolumes", "Lesson19"), ("Lesson20_MathLibrary", "Lesson20")
    ],
    "Lessons21-40_OpenGL": [
        ("Lesson21_OpenGLSetup", "Lesson21"), ("Lesson22_GLFWWindow", "Lesson22"),
        ("Lesson23_Pipeline", "Lesson23"), ("Lesson24_VBOandVAO", "Lesson24"),
        ("Lesson25_FirstTriangle", "Lesson25"), ("Lesson26_ShadersIntro", "Lesson26"),
        ("Lesson27_VertexShader", "Lesson27"), ("Lesson28_FragmentShader", "Lesson28"),
        ("Lesson29_ShaderPrograms", "Lesson29"), ("Lesson30_Texturing", "Lesson30"),
        ("Lesson31_TextureFiltering", "Lesson31"), ("Lesson32_MultipleTextures", "Lesson32"),
        ("Lesson33_Transformations", "Lesson33"), ("Lesson34_Camera", "Lesson34"),
        ("Lesson35_PhongLighting", "Lesson35"), ("Lesson36_Materials", "Lesson36"),
        ("Lesson37_MultipleLights", "Lesson37"), ("Lesson38_ModelLoading", "Lesson38"),
        ("Lesson39_DepthTesting", "Lesson39"), ("Lesson40_Blending", "Lesson40")
    ],
    "Lessons41-60_DirectX": [
        ("Lesson41_DirectXIntro", "Lesson41"), ("Lesson42_D3D11Setup", "Lesson42"),
        ("Lesson43_SwapChain", "Lesson43"), ("Lesson44_DeviceContext", "Lesson44"),
        ("Lesson45_VertexBuffers", "Lesson45"), ("Lesson46_IndexBuffers", "Lesson46"),
        ("Lesson47_HLSLShaders", "Lesson47"), ("Lesson48_ConstantBuffers", "Lesson48"),
        ("Lesson49_InputLayouts", "Lesson49"), ("Lesson50_TexturesDX", "Lesson50"),
        ("Lesson51_DepthStencil", "Lesson51"), ("Lesson52_RasterizerState", "Lesson52"),
        ("Lesson53_BlendStates", "Lesson53"), ("Lesson54_DirectXMath", "Lesson54"),
        ("Lesson55_CameraDX", "Lesson55"), ("Lesson56_LightingDX", "Lesson56"),
        ("Lesson57_NormalMapping", "Lesson57"), ("Lesson58_ShadowMapping", "Lesson58"),
        ("Lesson59_RenderToTexture", "Lesson59"), ("Lesson60_DirectX12Intro", "Lesson60")
    ],
    "Lessons61-80_Advanced": [
        ("Lesson61_DeferredRendering", "Lesson61"), ("Lesson62_SSAO", "Lesson62"),
        ("Lesson63_PBRTheory", "Lesson63"), ("Lesson64_PBRImplementation", "Lesson64"),
        ("Lesson65_IBL", "Lesson65"), ("Lesson66_HDR", "Lesson66"),
        ("Lesson67_Bloom", "Lesson67"), ("Lesson68_ShadowsAdvanced", "Lesson68"),
        ("Lesson69_Cubemaps", "Lesson69"), ("Lesson70_Reflections", "Lesson70"),
        ("Lesson71_GeometryShader", "Lesson71"), ("Lesson72_Tessellation", "Lesson72"),
        ("Lesson73_ComputeShader", "Lesson73"), ("Lesson74_Instancing", "Lesson74"),
        ("Lesson75_LOD", "Lesson75"), ("Lesson76_FrustumCullingGPU", "Lesson76"),
        ("Lesson77_OcclusionCulling", "Lesson77"), ("Lesson78_PostProcessing", "Lesson78"),
        ("Lesson79_AntiAliasing", "Lesson79"), ("Lesson80_ParticleSystems", "Lesson80")
    ],
    "Lessons81-100_Modern": [
        ("Lesson81_AdvancedShaders", "Lesson81"), ("Lesson82_ProceduralTextures", "Lesson82"),
        ("Lesson83_WaterRendering", "Lesson83"), ("Lesson84_TerrainRendering", "Lesson84"),
        ("Lesson85_Vegetation", "Lesson85"), ("Lesson86_VolumetricLighting", "Lesson86"),
        ("Lesson87_Fog", "Lesson87"), ("Lesson88_RayTracing", "Lesson88"),
        ("Lesson89_DXR", "Lesson89"), ("Lesson90_PathTracing", "Lesson90"),
        ("Lesson91_Vulkan", "Lesson91"), ("Lesson92_GraphicsAPIs", "Lesson92"),
        ("Lesson93_GPUOptimization", "Lesson93"), ("Lesson94_MemoryManagement", "Lesson94"),
        ("Lesson95_MultithreadedRendering", "Lesson95"), ("Lesson96_3DEngine", "Lesson96"),
        ("Lesson97_GameRenderer", "Lesson97"), ("Lesson98_MaterialEditor", "Lesson98"),
        ("Lesson99_BestPractices", "Lesson99"), ("Lesson100_FutureRendering", "Lesson100")
    ]
}

# Part 4: Optimization & Advanced - 100 lessons
part4_lessons = [
    ("Lesson01_PerfIntro", "Lesson01"), ("Lesson02_Benchmarking", "Lesson02"),
    ("Lesson03_VSProfiler", "Lesson03"), ("Lesson04_VTune", "Lesson04"),
    ("Lesson05_PerfCounters", "Lesson05"), ("Lesson06_MemoryProfiling", "Lesson06"),
    ("Lesson07_CacheOptimization", "Lesson07"), ("Lesson08_BranchPrediction", "Lesson08"),
    ("Lesson09_SIMD", "Lesson09"), ("Lesson10_CompilerOptimization", "Lesson10"),
    ("Lesson11_PGO", "Lesson11"), ("Lesson12_LTO", "Lesson12"),
    ("Lesson13_HotPath", "Lesson13"), ("Lesson14_DataOrientedDesign", "Lesson14"),
    ("Lesson15_MemoryLayout", "Lesson15"), ("Lesson16_CustomAllocators", "Lesson16"),
    ("Lesson17_ObjectPool", "Lesson17"), ("Lesson18_SmallStringOpt", "Lesson18"),
    ("Lesson19_CopyElision", "Lesson19"), ("Lesson20_PerfTestingFramework", "Lesson20"),
    ("Lesson21_BigOAnalysis", "Lesson21"), ("Lesson22_DataStructures", "Lesson22"),
    ("Lesson23_StringOptimization", "Lesson23"), ("Lesson24_LoopOptimization", "Lesson24"),
    ("Lesson25_Inlining", "Lesson25"), ("Lesson26_VirtualCallOpt", "Lesson26"),
    ("Lesson27_TemplateMetaprogramming", "Lesson27"), ("Lesson28_Constexpr", "Lesson28"),
    ("Lesson29_ZeroCost", "Lesson29"), ("Lesson30_MemoryAllocation", "Lesson30"),
    ("Lesson31_LazyEvaluation", "Lesson31"), ("Lesson32_ExpressionTemplates", "Lesson32"),
    ("Lesson33_CRTP", "Lesson33"), ("Lesson34_TypeErasure", "Lesson34"),
    ("Lesson35_SmallObjectOpt", "Lesson35"), ("Lesson36_FastMath", "Lesson36"),
    ("Lesson37_BitManipulation", "Lesson37"), ("Lesson38_LookupTables", "Lesson38"),
    ("Lesson39_BranchMinimization", "Lesson39"), ("Lesson40_OptimizationCases", "Lesson40"),
    ("Lesson41_ConcurrencyIntro", "Lesson41"), ("Lesson42_StdThread", "Lesson42"),
    ("Lesson43_ThreadManagement", "Lesson43"), ("Lesson44_RaceConditions", "Lesson44"),
    ("Lesson45_MutexLocks", "Lesson45"), ("Lesson46_LockFree", "Lesson46"),
    ("Lesson47_Atomics", "Lesson47"), ("Lesson48_MemoryOrdering", "Lesson48"),
    ("Lesson49_ConditionVariables", "Lesson49"), ("Lesson50_Semaphores", "Lesson50"),
    ("Lesson51_ThreadPool", "Lesson51"), ("Lesson52_WorkStealing", "Lesson52"),
    ("Lesson53_ProducerConsumer", "Lesson53"), ("Lesson54_ReadersWriters", "Lesson54"),
    ("Lesson55_AsyncFuture", "Lesson55"), ("Lesson56_Promises", "Lesson56"),
    ("Lesson57_ParallelAlgorithms", "Lesson57"), ("Lesson58_TaskParallelism", "Lesson58"),
    ("Lesson59_SIMDThreading", "Lesson59"), ("Lesson60_ConcurrentDataStructures", "Lesson60"),
    ("Lesson61_PatternsIntro", "Lesson61"), ("Lesson62_Singleton", "Lesson62"),
    ("Lesson63_Factory", "Lesson63"), ("Lesson64_AbstractFactory", "Lesson64"),
    ("Lesson65_Builder", "Lesson65"), ("Lesson66_Prototype", "Lesson66"),
    ("Lesson67_Adapter", "Lesson67"), ("Lesson68_Bridge", "Lesson68"),
    ("Lesson69_Composite", "Lesson69"), ("Lesson70_Decorator", "Lesson70"),
    ("Lesson71_Facade", "Lesson71"), ("Lesson72_Proxy", "Lesson72"),
    ("Lesson73_ChainOfResponsibility", "Lesson73"), ("Lesson74_Command", "Lesson74"),
    ("Lesson75_Iterator", "Lesson75"), ("Lesson76_Observer", "Lesson76"),
    ("Lesson77_State", "Lesson77"), ("Lesson78_Strategy", "Lesson78"),
    ("Lesson79_TemplateMethod", "Lesson79"), ("Lesson80_Visitor", "Lesson80"),
    ("Lesson81_Architecture", "Lesson81"), ("Lesson82_DataProcessingEngine1", "Lesson82"),
    ("Lesson83_DataProcessingEngine2", "Lesson83"), ("Lesson84_GameEngine1", "Lesson84"),
    ("Lesson85_GameEngine2", "Lesson85"), ("Lesson86_NetworkServer", "Lesson86"),
    ("Lesson87_AnalyticsSystem", "Lesson87"), ("Lesson88_MemoryDatabase", "Lesson88"),
    ("Lesson89_RenderingEngine", "Lesson89"), ("Lesson90_MLFramework", "Lesson90"),
    ("Lesson91_CodeReview", "Lesson91"), ("Lesson92_Testing", "Lesson92"),
    ("Lesson93_CICD", "Lesson93"), ("Lesson94_Documentation", "Lesson94"),
    ("Lesson95_CrossPlatform", "Lesson95"), ("Lesson96_BuildSystems", "Lesson96"),
    ("Lesson97_AdvancedDebugging", "Lesson97"), ("Lesson98_Security", "Lesson98"),
    ("Lesson99_Refactoring", "Lesson99"), ("Lesson100_CareerFinal", "Lesson100")
]

def generate_solution():
    """Generate the complete solution file"""

    sln_content = []

    # Header
    sln_content.append("")
    sln_content.append("Microsoft Visual Studio Solution File, Format Version 12.00")
    sln_content.append("# Visual Studio Version 17")
    sln_content.append("VisualStudioVersion = 17.0.31903.59")
    sln_content.append("MinimumVisualStudioVersion = 10.0.40219.1")
    sln_content.append("")
    sln_content.append("# ==============================================================================")
    sln_content.append("# C++ Tutorial Course - COMPLETE 400 Hour Course")
    sln_content.append("# All 370+ Lessons with Executable Code")
    sln_content.append("# ==============================================================================")
    sln_content.append("#")
    sln_content.append("# Part 1: C++ Fundamentals (80 Lessons)")
    sln_content.append("# Part 2: MFC UI Development (90 Lessons)")
    sln_content.append("# Part 3: 3D Rendering (100 Lessons)")
    sln_content.append("# Part 4: Optimization & Advanced (100 Lessons)")
    sln_content.append("#")
    sln_content.append("# Total: 370 Lessons")
    sln_content.append("# ==============================================================================")
    sln_content.append("")

    # Solution Folders
    sln_content.append("# ==============================================================================")
    sln_content.append("# SOLUTION FOLDERS")
    sln_content.append("# ==============================================================================")
    sln_content.append("")

    folder_guids = {
        "Part1": "{F0000001-0000-0000-0000-000000000001}",
        "Part2": "{F0000002-0000-0000-0000-000000000002}",
        "Part3": "{F0000003-0000-0000-0000-000000000003}",
        "Part3_Math": "{F0000031-0000-0000-0000-000000000031}",
        "Part3_OpenGL": "{F0000032-0000-0000-0000-000000000032}",
        "Part3_DirectX": "{F0000033-0000-0000-0000-000000000033}",
        "Part3_Advanced": "{F0000034-0000-0000-0000-000000000034}",
        "Part3_Modern": "{F0000035-0000-0000-0000-000000000035}",
        "Part4": "{F0000004-0000-0000-0000-000000000004}",
    }

    sln_content.append(f'Project("{{2150E333-8FDC-42A3-9474-1A3956D46DE8}}") = "Part1-Fundamentals", "Part1-Fundamentals", "{folder_guids["Part1"]}"')
    sln_content.append("EndProject")
    sln_content.append("")
    sln_content.append(f'Project("{{2150E333-8FDC-42A3-9474-1A3956D46DE8}}") = "Part2-MFC-UI", "Part2-MFC-UI", "{folder_guids["Part2"]}"')
    sln_content.append("EndProject")
    sln_content.append("")
    sln_content.append(f'Project("{{2150E333-8FDC-42A3-9474-1A3956D46DE8}}") = "Part3-3D-Rendering", "Part3-3D-Rendering", "{folder_guids["Part3"]}"')
    sln_content.append("EndProject")
    sln_content.append("")
    sln_content.append(f'Project("{{2150E333-8FDC-42A3-9474-1A3956D46DE8}}") = "01-3DMath", "01-3DMath", "{folder_guids["Part3_Math"]}"')
    sln_content.append("EndProject")
    sln_content.append("")
    sln_content.append(f'Project("{{2150E333-8FDC-42A3-9474-1A3956D46DE8}}") = "02-OpenGL", "02-OpenGL", "{folder_guids["Part3_OpenGL"]}"')
    sln_content.append("EndProject")
    sln_content.append("")
    sln_content.append(f'Project("{{2150E333-8FDC-42A3-9474-1A3956D46DE8}}") = "03-DirectX", "03-DirectX", "{folder_guids["Part3_DirectX"]}"')
    sln_content.append("EndProject")
    sln_content.append("")
    sln_content.append(f'Project("{{2150E333-8FDC-42A3-9474-1A3956D46DE8}}") = "04-Advanced", "04-Advanced", "{folder_guids["Part3_Advanced"]}"')
    sln_content.append("EndProject")
    sln_content.append("")
    sln_content.append(f'Project("{{2150E333-8FDC-42A3-9474-1A3956D46DE8}}") = "05-Modern", "05-Modern", "{folder_guids["Part3_Modern"]}"')
    sln_content.append("EndProject")
    sln_content.append("")
    sln_content.append(f'Project("{{2150E333-8FDC-42A3-9474-1A3956D46DE8}}") = "Part4-Optimization-Advanced", "Part4-Optimization-Advanced", "{folder_guids["Part4"]}"')
    sln_content.append("EndProject")
    sln_content.append("")

    # Part 1 Projects
    sln_content.append("# ==============================================================================")
    sln_content.append("# PART 1: C++ FUNDAMENTALS (80 Lessons)")
    sln_content.append("# ==============================================================================")
    sln_content.append("")

    project_configs = []
    nested_projects = []

    for idx, lesson in enumerate(part1_lessons, 1):
        guid = f"{{10{idx:06d}-0000-0000-0000-{idx:012d}}}"
        sln_content.append(f'Project("{{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}}") = "{lesson}", "Part1-Fundamentals\\{lesson}.vcxproj", "{guid}"')
        sln_content.append("EndProject")
        sln_content.append("")
        project_configs.append((guid, lesson))
        nested_projects.append((guid, folder_guids["Part1"]))

    # Part 2 Projects
    sln_content.append("# ==============================================================================")
    sln_content.append("# PART 2: MFC UI DEVELOPMENT (90 Lessons)")
    sln_content.append("# ==============================================================================")
    sln_content.append("")

    for idx, (lesson_folder, project_name) in enumerate(part2_lessons, 1):
        guid = f"{{20{idx:06d}-0000-0000-0000-{idx:012d}}}"
        sln_content.append(f'Project("{{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}}") = "{lesson_folder}", "Part2-MFC-UI\\{lesson_folder}\\{project_name}.vcxproj", "{guid}"')
        sln_content.append("EndProject")
        sln_content.append("")
        project_configs.append((guid, lesson_folder))
        nested_projects.append((guid, folder_guids["Part2"]))

    # Part 3 Projects
    sln_content.append("# ==============================================================================")
    sln_content.append("# PART 3: 3D RENDERING (100 Lessons)")
    sln_content.append("# ==============================================================================")
    sln_content.append("")

    lesson_num = 1
    for category, lessons in part3_lessons.items():
        folder_map = {
            "Lessons01-20_3DMath": "Part3_Math",
            "Lessons21-40_OpenGL": "Part3_OpenGL",
            "Lessons41-60_DirectX": "Part3_DirectX",
            "Lessons61-80_Advanced": "Part3_Advanced",
            "Lessons81-100_Modern": "Part3_Modern"
        }

        sln_content.append(f"# {category}")
        sln_content.append("")

        for lesson_folder, project_name in lessons:
            guid = f"{{30{lesson_num:06d}-0000-0000-0000-{lesson_num:012d}}}"
            sln_content.append(f'Project("{{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}}") = "{lesson_folder}", "Part3-3D-Rendering\\{category}\\{lesson_folder}\\{project_name}.vcxproj", "{guid}"')
            sln_content.append("EndProject")
            sln_content.append("")
            project_configs.append((guid, lesson_folder))
            nested_projects.append((guid, folder_guids[folder_map[category]]))
            lesson_num += 1

    # Part 4 Projects
    sln_content.append("# ==============================================================================")
    sln_content.append("# PART 4: OPTIMIZATION & ADVANCED (100 Lessons)")
    sln_content.append("# ==============================================================================")
    sln_content.append("")

    for idx, (lesson_folder, project_name) in enumerate(part4_lessons, 1):
        guid = f"{{40{idx:06d}-0000-0000-0000-{idx:012d}}}"
        sln_content.append(f'Project("{{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}}") = "{lesson_folder}", "Part4-Optimization-Advanced\\{lesson_folder}\\{project_name}.vcxproj", "{guid}"')
        sln_content.append("EndProject")
        sln_content.append("")
        project_configs.append((guid, lesson_folder))
        nested_projects.append((guid, folder_guids["Part4"]))

    # Global section
    sln_content.append("Global")
    sln_content.append("\tGlobalSection(SolutionConfigurationPlatforms) = preSolution")
    sln_content.append("\t\tDebug|x64 = Debug|x64")
    sln_content.append("\t\tRelease|x64 = Release|x64")
    sln_content.append("\tEndGlobalSection")
    sln_content.append("")

    # Project configurations
    sln_content.append("\tGlobalSection(ProjectConfigurationPlatforms) = postSolution")
    for guid, name in project_configs:
        sln_content.append(f"\t\t{guid}.Debug|x64.ActiveCfg = Debug|x64")
        sln_content.append(f"\t\t{guid}.Debug|x64.Build.0 = Debug|x64")
        sln_content.append(f"\t\t{guid}.Release|x64.ActiveCfg = Release|x64")
        sln_content.append(f"\t\t{guid}.Release|x64.Build.0 = Release|x64")
    sln_content.append("\tEndGlobalSection")
    sln_content.append("")

    # Solution properties
    sln_content.append("\tGlobalSection(SolutionProperties) = preSolution")
    sln_content.append("\t\tHideSolutionNode = FALSE")
    sln_content.append("\tEndGlobalSection")
    sln_content.append("")

    # Nested projects
    sln_content.append("\tGlobalSection(NestedProjects) = preSolution")
    for project_guid, folder_guid in nested_projects:
        sln_content.append(f"\t\t{project_guid} = {folder_guid}")

    # Nest subfolder under main Part3 folder
    sln_content.append(f'\t\t{folder_guids["Part3_Math"]} = {folder_guids["Part3"]}')
    sln_content.append(f'\t\t{folder_guids["Part3_OpenGL"]} = {folder_guids["Part3"]}')
    sln_content.append(f'\t\t{folder_guids["Part3_DirectX"]} = {folder_guids["Part3"]}')
    sln_content.append(f'\t\t{folder_guids["Part3_Advanced"]} = {folder_guids["Part3"]}')
    sln_content.append(f'\t\t{folder_guids["Part3_Modern"]} = {folder_guids["Part3"]}')
    sln_content.append("\tEndGlobalSection")
    sln_content.append("")

    # Extensibility Globals
    sln_content.append("\tGlobalSection(ExtensibilityGlobals) = postSolution")
    sln_content.append("\t\tSolutionGuid = {FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF}")
    sln_content.append("\tEndGlobalSection")
    sln_content.append("")

    sln_content.append("EndGlobal")
    sln_content.append("")

    return "\n".join(sln_content)

def generate_solution_filter(name, description, project_guids):
    """Generate a solution filter file"""
    filter_data = {
        "solution": {
            "path": "CPP-Tutorial-Complete.sln",
            "projects": project_guids
        }
    }
    return json.dumps(filter_data, indent=2)

if __name__ == "__main__":
    # Generate main solution
    print("Generating CPP-Tutorial-Complete.sln...")
    sln_content = generate_solution()

    with open("CPP-Tutorial-Complete.sln", "w", encoding="utf-8") as f:
        f.write(sln_content)

    print("Solution file generated successfully!")
    print("\nSummary:")
    print("  Part 1 (Fundamentals): 80 projects")
    print("  Part 2 (MFC UI): 90 projects")
    print("  Part 3 (3D Rendering): 100 projects")
    print("  Part 4 (Optimization): 100 projects")
    print("  Total: 370 projects")
    print("\nSolution features:")
    print("  - Organized in solution folders")
    print("  - Debug and Release configurations")
    print("  - x64 platform target")
    print("  - Professional structure")

    # Generate solution filters
    print("\nGenerating solution filters...")

    # Beginners filter (Part 1, Lessons 1-30)
    beginners_guids = [f"{{10{i:06d}-0000-0000-0000-{i:012d}}}" for i in range(1, 31)]
    with open("Beginners.slnf", "w") as f:
        f.write(generate_solution_filter("Beginners", "Part 1 Lessons 1-30", beginners_guids))
    print("  - Beginners.slnf (30 projects)")

    # MFC Basics filter (Part 2, Lessons 1-30)
    mfc_basics_guids = [f"{{20{i:06d}-0000-0000-0000-{i:012d}}}" for i in range(1, 31)]
    with open("MFC-Basics.slnf", "w") as f:
        f.write(generate_solution_filter("MFC Basics", "Part 2 Lessons 1-30", mfc_basics_guids))
    print("  - MFC-Basics.slnf (30 projects)")

    # 3D Basics filter (Part 3, Lessons 1-40)
    threed_basics_guids = [f"{{30{i:06d}-0000-0000-0000-{i:012d}}}" for i in range(1, 41)]
    with open("3D-Basics.slnf", "w") as f:
        f.write(generate_solution_filter("3D Basics", "Part 3 Lessons 1-40", threed_basics_guids))
    print("  - 3D-Basics.slnf (40 projects)")

    # Optimization filter (Part 4, Lessons 1-40)
    optimization_guids = [f"{{40{i:06d}-0000-0000-0000-{i:012d}}}" for i in range(1, 41)]
    with open("Optimization.slnf", "w") as f:
        f.write(generate_solution_filter("Optimization", "Part 4 Lessons 1-40", optimization_guids))
    print("  - Optimization.slnf (40 projects)")

    print("\nAll files generated successfully!")
    print("Open CPP-Tutorial-Complete.sln in Visual Studio 2019/2022")
