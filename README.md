# ![xr_viewer](https://github.com/PavelBlend/xr_viewer/blob/master/icon/icon_32.png) X-Ray Viewer
![screenshot](https://ap-pro.ru/uploads/monthly_2026_10/xr_viewer.png.82ec5f521bb099cc371bd6525f0c6519.png)
Программа для просмотра `*.ogf` 3D моделей из игры `STALKER`. Поддерживаются файлы версии `4` из `ТЧ`, `ЧН`, `ЗП` и из билдов новее `1865`. Чтобы отображались текстуры, нужно создать виртуальный диск `T`, который ссылается на папку `gamedata\textures` с `*.dds` файлами. Для создания и удаления виртуального диска `T` можно использовать файлы `Disk_T_Create.bat` и `Disk_T_Remove.bat` из этого репозитория.

## Использование
`xr_viewer.exe <путь_к_ogf_файлу>`

Либо можно ассоциировать `*.ogf` файлы с программой и открывать их двойным щелчком мыши.

## Навигация
- Левая кнопка мыши - вращение вида
- Зажатое колесо мыши - перемещение вида
- Прокрутка колеса мыши - изменение масштаба вида

## Горячие клавиши
- `Escape` - закрыть программу
- `W` - отображать рёбра (wireframe)
- `F` - отображать полигоны (faces)
- `P` - отображать точки (points)
- `G` - отображать координатную сетку (grid)
- `R` - рандомизировать цвета мешей (random color)
- `S` - установить один цвет всем мешам (single color)
- `T` - отображать текустуры (textures)
- `B` - отображать ограничивающие коробки (bounding box)
- `C` - центрировать вид по модели (center view)

## Компиляция
Для компиляции нужно запустить `build.bat` файл.

После компиляции двоичные файлы программы скопируются в `C:\progs\xr_viewer\`.

## Зависимости
- [MinGW-W64](https://github.com/niXman/mingw-builds-binaries)
- [CMake 4.2](https://github.com/Kitware/CMake/releases/tag/v4.2.8)
- [Premake 5.0.0](https://github.com/premake/premake-core/releases/tag/v5.0.0)
- [ImageMagick](https://github.com/ImageMagick/ImageMagick)
- [GLFW 3.3.9](https://github.com/glfw/glfw/releases/tag/3.3.9)
- [GLEW 2.2.0](https://github.com/nigels-com/glew/releases/tag/glew-2.2.0)
- [CGLM 0.9.6](https://github.com/recp/cglm/releases/tag/v0.9.6)
- [SOIL2 1.4.2](https://github.com/SpartanJ/SOIL2/releases/tag/1.4.2)
- [cimgui](https://github.com/cimgui/cimgui)

В `CMakeLists.txt` указаны такие абсолютные пути к зависимостям:
- MinGW-W64 - `C:\progs\mingw\bin\`
- ImageMagick - `C:\progs\image_magick\`
- Premake - `C:\progs\premake\`
- GLFW - `C:\libs\glfw\glfw-3.3.9\`
- GLEW - `C:\libs\glew\glew-2.2.0\`
- CGLM - `C:\libs\cglm\cglm-0.9.6\`
- SOIL2 - `C:\libs\soil2\SOIL2-1.4.2\`
- cimgui - `C:\libs\cimgui\cimgui\`
