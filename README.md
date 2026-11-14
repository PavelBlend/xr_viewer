# ![xr_viewer](https://github.com/PavelBlend/xr_viewer/blob/master/icon/icon_32.png) X-Ray Viewer
![screenshot](https://ap-pro.ru/uploads/monthly_2026_10/xr_viewer.png.82ec5f521bb099cc371bd6525f0c6519.png)
Программа для просмотра `*.ogf` 3D моделей из игры `STALKER`. Поддерживаются файлы версии `4` из `ТЧ`, `ЧН`, `ЗП` и из билдов новее `1865`. Чтобы отображались текстуры, нужно создать виртуальный диск `T`, который ссылается на папку `gamedata\textures` с `*.dds` файлами. Для создания и удаления виртуального диска `T` можно использовать файлы `Create_Disk_T.bat` и `Remove_Disk_T.bat` из этого репозитория.

## Использование
`xr_viewer.exe <путь_к_ogf_файлу>`

Либо можно ассоциировать `*.ogf` файлы с программой и открывать их двойным щелчком мыши.

## Навигация
- Левая кнопка мыши - вращение вида
- Зажатое колесо мыши - перемещение вида
- Прокрутка колеса мыши - изменение масштаба вида

## Компиляция
Для компиляции нужно запустить `build.bat` файл.

После компиляции двоичные файлы программы скопируются в `C:\progs\xr_viewer\`.

## Зависимости
- [MinGW-W64](https://github.com/niXman/mingw-builds-binaries)
- [CMake 4.2](https://github.com/Kitware/CMake/releases/tag/v4.2.8)
- [ImageMagick](https://github.com/ImageMagick/ImageMagick)
- [GLFW 3.3.9](https://github.com/glfw/glfw/releases/tag/3.3.9)
- [GLEW 2.2.0](https://github.com/nigels-com/glew/releases/tag/glew-2.2.0)
- [CGLM 0.9.6](https://github.com/recp/cglm/releases/tag/v0.9.6)
- [FreeImage 3.18.0](https://sourceforge.net/projects/freeimage/files/Binary%20Distribution/3.18.0/)
- [cimgui](https://github.com/cimgui/cimgui)

В `CMakeLists.txt` указаны такие абсолютные пути к зависимостям:
- MinGW-W64 - `C:\progs\mingw\bin\`
- ImageMagick - `C:\progs\image_magick\`
- GLFW - `C:\libs\glfw\glfw-3.3.9\`
- GLEW - `C:\libs\glew\glew-2.2.0\`
- CGLM - `C:\libs\cglm\cglm-0.9.6\`
- FreeImage - `C:\libs\freeimage\FreeImage3180Win32Win64\FreeImage\`
- cimgui - `C:\libs\cimgui\cimgui\`
