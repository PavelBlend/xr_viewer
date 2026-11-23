#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define GLFW_EXPOSE_NATIVE_WIN32

// Стандартная библиотека
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>
#include <float.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>
#include <wchar.h>
#include <shellapi.h>

// Внешние зависимости
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include <GLFW\glfw3native.h>
#include "cglm\cglm.h"
#include "SOIL2.h"
#include "cimgui.h"
#include "cimgui_impl.h"

// Локальные модули
#include "render.c"
#include "app\app.c"

// Главная функция программы
int main (int argc, char* argv[]) {

    // Создать основную структуру программы
    Application app = create_app(argc, argv);

    // Рендер
    render(&app);

    // Освободить память программы
    free_mem(&app);

    return 0;

}
