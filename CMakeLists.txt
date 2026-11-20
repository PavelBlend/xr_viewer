# версия cmake
cmake_minimum_required(VERSION 3.10)

# проект
project(xr_viewer)

# настройки языка си++
set(CMAKE_CXX_STANDARD 11)

# установить путь для исполняемых файлов
set(BIN_DIRECTORY "C:/progs/xr_viewer")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY          ${BIN_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG    ${BIN_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BIN_DIRECTORY})

# установить путь к программам
set(PROGS_DIRECTORY "C:/progs/")

# найти программу magick
find_program(MAGICK_EXECUTABLE
    NAMES magick convert
    HINTS "${PROGS_DIRECTORY}image_magick"
)

# найти программу windres
find_program(WINDRES_EXECUTABLE
    NAMES windres
    HINTS "${PROGS_DIRECTORY}mingw/bin"
)

# Проверяем, что всё нашли
if(NOT MAGICK_EXECUTABLE)
    message(WARNING "ImageMagick (magick/convert) not found")
endif()

if(NOT WINDRES_EXECUTABLE)
    message(FATAL_ERROR "windres not found")
endif()

# пути к файлам иконки
set(ICON_DIR          "${CMAKE_SOURCE_DIR}/icon/")
set(ICON_O            "${ICON_DIR}icon.o")
set(ICON_RC           "${ICON_DIR}icon.rc")
set(ICON_ICO          "${ICON_DIR}icon.ico")

# имя иконки
set(ICON_NAME "icon_")

# расширение иконки
set(ICON_EXT ".png")

# список png-файлов для иконки
set(ICON_PNGS
    "${ICON_DIR}/${ICON_NAME}16${ICON_EXT}"
    "${ICON_DIR}/${ICON_NAME}32${ICON_EXT}"
    "${ICON_DIR}/${ICON_NAME}48${ICON_EXT}"
    "${ICON_DIR}/${ICON_NAME}64${ICON_EXT}"
    "${ICON_DIR}/${ICON_NAME}128${ICON_EXT}"
    "${ICON_DIR}/${ICON_NAME}256${ICON_EXT}"
)

# конвертировать png в ico
add_custom_command(
    OUTPUT ${ICON_ICO}
    COMMAND ${MAGICK_EXECUTABLE} ${ICON_PNGS} ${ICON_ICO}
    DEPENDS ${ICON_PNGS}
    COMMENT "Convert PNG - icon.ico (magick)"
    VERBATIM
)

# компилировать иконку
add_custom_command(
    OUTPUT ${ICON_O}
    COMMAND ${WINDRES_EXECUTABLE} -F pe-x86-64 ${ICON_RC} -o ${ICON_O}
    DEPENDS ${ICON_RC} ${ICON_ICO} ${ICON_PNGS}
    COMMENT "Compile icon.rc - icon.o (windres)"
    VERBATIM
)

# генерация иконки
add_custom_target(GenerateIcon
    DEPENDS ${ICON_O}
    COMMENT "icon generation"
)

# исходные файлы
set(SOURCES "src/main.c")

# ресурсы
set(RESOURCES "icon/icon.o")

# исполняемый файл
add_executable(xr_viewer ${SOURCES} ${RESOURCES})

add_dependencies(xr_viewer GenerateIcon)

# корневой путь к зависимостям
set(LIBS_DIR            "C:/libs/")

# пути к зависимостям
set(GLFW_DIR            "${LIBS_DIR}glfw/glfw-3.3.9/")
set(GLEW_DIR            "${LIBS_DIR}glew/glew-2.2.0/")
set(CGLM_DIR            "${LIBS_DIR}cglm/cglm-0.9.6/")
set(FREE_IMAGE_DIR      "${LIBS_DIR}freeimage/FreeImage3180Win32Win64/FreeImage/")

# пути к включениям
set(GLFW_INCLUDE            "${GLFW_DIR}include")
set(GLEW_INCLUDE            "${GLEW_DIR}include")
set(CGLM_INCLUDE            "${CGLM_DIR}include")
set(FREE_IMAGE_INCLUDE      "${FREE_IMAGE_DIR}Dist/x64")
set(CIMGUI_INCLUDE          "${LIBS_DIR}cimgui/cimgui")

# пути к библиотекам
set(GLFW_LIB            "${GLFW_DIR}lib-mingw-w64")
set(GLEW_LIB            "${GLEW_DIR}lib/Release/x64")
set(FREE_IMAGE_LIB      "${FREE_IMAGE_DIR}Dist/x64")

# установить пути к включениям для xr_viewer
target_include_directories(xr_viewer PRIVATE
    ${GLFW_INCLUDE}
    ${GLEW_INCLUDE}
    ${CGLM_INCLUDE}
    ${FREE_IMAGE_INCLUDE}
    ${CIMGUI_INCLUDE}
)

# установить пути к библиотекам для xr_viewer
target_link_directories(xr_viewer PRIVATE
    ${GLFW_LIB}
    ${GLEW_LIB}
    ${FREE_IMAGE_LIB}
)

# установить путь к imgui (Си++)
set(IMGUI_FOLDER "${CIMGUI_INCLUDE}/imgui/")

set(BACKENDS_FOLDER "${IMGUI_FOLDER}backends/"         )
set(TABLES_SOURCE   "${IMGUI_FOLDER}imgui_tables.cpp"  )

include_directories("${CIMGUI_INCLUDE}/imgui")

add_definitions("-DIMGUI_USER_CONFIG=\"C:/libs/cimgui/cimgui/cimconfig.h\"")
add_definitions("-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1")

set(IMGUI_SOURCES 
    "${CIMGUI_INCLUDE}/cimgui.cpp"
    "${IMGUI_FOLDER}imgui.cpp"
    "${IMGUI_FOLDER}imgui_draw.cpp"
    "${IMGUI_FOLDER}imgui_demo.cpp"
    "${IMGUI_FOLDER}imgui_widgets.cpp"
    ${TABLES_SOURCE}
)

add_compile_definitions("IMGUI_IMPL_OPENGL_LOADER_GL3W")

# opengl 3
list(APPEND IMGUI_SOURCES ${BACKENDS_FOLDER}imgui_impl_opengl3.cpp)

# GLFW
list(APPEND IMGUI_SOURCES ${BACKENDS_FOLDER}imgui_impl_glfw.cpp)

add_library(cimgui SHARED ${IMGUI_SOURCES})

# установить пути к включениям для cimgui
target_include_directories(cimgui PRIVATE
    ${GLFW_INCLUDE}
)

# установить пути к библиотекам для cimgui
target_link_directories(cimgui PRIVATE
    ${GLFW_LIB}
)

# использовать библиотеки для cimgui
target_link_libraries(cimgui PRIVATE
    glfw3dll
)

target_compile_definitions(cimgui PUBLIC "-DIMGUI_IMPL_API=extern \"C\" __declspec\(dllexport\)")

# using library
target_compile_definitions(${PROJECT_NAME} PUBLIC -DCIMGUI_USE_OPENGL3 -DCIMGUI_USE_GLFW)

# опции компилятора
target_compile_options(xr_viewer PRIVATE -Wall -m64)

# использовать библиотеки для xr_viewer
target_link_libraries(xr_viewer PRIVATE
    glfw3dll
    glew32
    FreeImage
    opengl32
    gdi32
    cimgui
)

# пути к dll файлам
set(GLFW_DLL            "${GLFW_LIB}/glfw3.dll")
set(GLEW_DLL            "${GLEW_DIR}/bin/Release/x64/glew32.dll")
set(FREEIMAGE_DLL       "${FREE_IMAGE_LIB}/FreeImage.dll")

# пути к glsl файлам
set(SHADERS_DIR             "${CMAKE_SOURCE_DIR}/shaders/")
set(VERTEX_SHADER_GLSL      "${SHADERS_DIR}vertex_shader.glsl")
set(FRAGMENT_SHADER_GLSL    "${SHADERS_DIR}fragment_shader.glsl")

# список копируемых dll файлов
set(COPY_DLL
    ${GLFW_DLL}
    ${GLEW_DLL}
    ${FREEIMAGE_DLL}
)

# список копируемых glsl файлов
set(COPY_GLSL
    ${VERTEX_SHADER_GLSL}
    ${FRAGMENT_SHADER_GLSL}
)

# копирование dll файлов
foreach(COPY_FILE IN LISTS COPY_DLL)
    cmake_path(GET COPY_FILE FILENAME FILE_NAME)
    add_custom_command(TARGET xr_viewer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo                "Copy: ${FILE_NAME} in $<TARGET_FILE_DIR:xr_viewer>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different   "${COPY_FILE}" "$<TARGET_FILE_DIR:xr_viewer>/${FILE_NAME}"
        VERBATIM
    )
endforeach()

# копирование glsl файлов
foreach(COPY_FILE IN LISTS COPY_GLSL)
    cmake_path(GET COPY_FILE FILENAME FILE_NAME)
    add_custom_command(TARGET xr_viewer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo                "Copy: ${FILE_NAME} in $<TARGET_FILE_DIR:xr_viewer>/shaders"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different   "${COPY_FILE}" "$<TARGET_FILE_DIR:xr_viewer>/shaders/${FILE_NAME}"
        VERBATIM
    )
endforeach()
