set(GLAD_DIR ${CMAKE_CURRENT_LIST_DIR}/glad)
add_library(glad STATIC ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)

