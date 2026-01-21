add_library(ImGui)

set(ImGui_header_files)
list(APPEND ImGui_header_files include/imgui.h)
list(APPEND ImGui_header_files include/imconfig.h)
list(APPEND ImGui_header_files include/imgui_internal.h)
list(APPEND ImGui_header_files include/imstb_rectpack.h)
list(APPEND ImGui_header_files include/imstb_textedit.h)
list(APPEND ImGui_header_files include/imstb_truetype.h)

set(ImGui_source_files)
list(APPEND ImGui_source_files src/imgui.cpp)
list(APPEND ImGui_source_files src/imgui_draw.cpp)
list(APPEND ImGui_source_files src/imgui_tables.cpp)
list(APPEND ImGui_source_files src/imgui_widgets.cpp)
list(APPEND ImGui_source_files src/imgui_demo.cpp)

target_sources(ImGui
        PRIVATE ${ImGui_source_files}
        PUBLIC
            FILE_SET    imgui_headers
            TYPE        HEADERS
            BASE_DIRS   ./include
            FILES       ${ImGui_header_files}
)
