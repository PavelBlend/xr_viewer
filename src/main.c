#include "render.c"
#include "app\application.c"

int main (int argc, char* argv[]) {

    // Создать основную структуру программы
    Application app = create_app(argc, argv);

    // Рендер
    render(&app);

    // Освободить память программы
    free_mem(&app);

    return 0;

}
