#include "render.c"
#include "application.c"

int main (int argc, char* argv[]) {

    // Создать основную структуру программы
    Application app;

    // Инициализировать основную структуру программы
    init_app(&app, argc, argv);

    // Рендер
    render(&app);

    // Освободить память программы
    free_mem(&app);

    return 0;

}
