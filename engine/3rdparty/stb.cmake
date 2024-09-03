file(GLOB STB_SOURCES  "${CMAKE_CURRENT_LIST_DIR}/stb/*.h")
add_library(stb INTERFACE ${STB_SOURCES})
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_LIST_DIR}/stb)
