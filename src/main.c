#include "file_io.c"
#include "chunk_io.c"
#include "ogf_io.c"
#include "render.c"
#include "application.c"
#include "cmd_utils.c"

int main (int argc, char* argv[]) {

    // Создать основную структуру программы
    Application app;

    // Инициализировать основную структуру программы
    init_app(&app);

    // Обработать аргументы командной строки
    handle_arguments(&app, argc, argv);

    // Рендер
    render(&app);

    // Освободить память программы
    free_mem(&app);

    return 0;

}
