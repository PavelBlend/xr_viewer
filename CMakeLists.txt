# версия cmake
cmake_minimum_required(VERSION 3.10)

# проект
project(xr_viewer LANGUAGES C)

# настройки языка си
set(CMAKE_C_STANDARD            11)
set(CMAKE_C_STANDARD_REQUIRED   ON)
set(CMAKE_C_EXTENSIONS          OFF)

# установить путь для исполняемых файлов
set(BIN_DIRECTORY "C:/progs/xr_viewer")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIRECTORY})

# найти программу magick
find_program(MAGICK_EXECUTABLE
    NAMES magick convert
    HINTS "C:/progs/image_magick"
)

# найти программу windres
find_program(WINDRES_EXECUTABLE
    NAMES windres
    HINTS "C:/progs/mingw/bin"
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

# список png-файлов для иконки
set(ICON_PNGS
    "${ICON_DIR}/icon_16.png"
    "${ICON_DIR}/icon_32.png"
    "${ICON_DIR}/icon_48.png"
    "${ICON_DIR}/icon_64.png"
    "${ICON_DIR}/icon_128.png"
    "${ICON_DIR}/icon_256.png"
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
set(SOURCES src/main.c)

# ресурсы
set(RESOURCES icon/icon.o)

# исполняемый файл
add_executable(xr_viewer ${SOURCES} ${RESOURCES})

add_dependencies(xr_viewer GenerateIcon)

# опции компилятора
target_compile_options(xr_viewer PRIVATE -Wall -m64)

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

# пути к библиотекам
set(GLFW_LIB            "${GLFW_DIR}lib-mingw-w64")
set(GLEW_LIB            "${GLEW_DIR}lib/Release/x64")
set(FREE_IMAGE_LIB      "${FREE_IMAGE_DIR}Dist/x64")

# установить пути к включениям
target_include_directories(xr_viewer PRIVATE
    ${GLFW_INCLUDE}
    ${GLEW_INCLUDE}
    ${CGLM_INCLUDE}
    ${FREE_IMAGE_INCLUDE}
)

# установить пути к библиотекам
target_link_directories(xr_viewer PRIVATE
    ${GLFW_LIB}
    ${GLEW_LIB}
    ${FREE_IMAGE_LIB}
)

# использовать библиотеки
target_link_libraries(xr_viewer PRIVATE
    glfw3dll
    glew32
    FreeImage
    opengl32
    gdi32
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
        COMMAND ${CMAKE_COMMAND} -E echo "Copy: ${FILE_NAME} in $<TARGET_FILE_DIR:xr_viewer>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${COPY_FILE}"
            "$<TARGET_FILE_DIR:xr_viewer>/${FILE_NAME}"
        VERBATIM
    )
endforeach()

# копирование glsl файлов
foreach(COPY_FILE IN LISTS COPY_GLSL)
    cmake_path(GET COPY_FILE FILENAME FILE_NAME)
    add_custom_command(TARGET xr_viewer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copy: ${FILE_NAME} in $<TARGET_FILE_DIR:xr_viewer>/shaders"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${COPY_FILE}"
            "$<TARGET_FILE_DIR:xr_viewer>/shaders/${FILE_NAME}"
        VERBATIM
    )
endforeach()
