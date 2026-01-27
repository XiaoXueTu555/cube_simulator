add_library(ImGui)

set(ImGui_header_files)
list(APPEND ImGui_header_files
        include/imgui.h
        include/imconfig.h
        include/imgui_internal.h
        include/imstb_rectpack.h
        include/imstb_textedit.h
        include/imstb_truetype.h
)

set(ImGui_source_files)
list(APPEND ImGui_source_files
        src/imgui.cpp
        src/imgui_draw.cpp
        src/imgui_tables.cpp
        src/imgui_widgets.cpp
        src/imgui_demo.cpp
)

target_sources(ImGui
        PRIVATE ${ImGui_source_files}
        PUBLIC
            FILE_SET    imgui_headers
            TYPE        HEADERS
            BASE_DIRS   ./include
            FILES       ${ImGui_header_files}
)
