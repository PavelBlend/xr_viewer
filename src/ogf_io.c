#ifndef OGF_IO_C
#define OGF_IO_C

#include <stdlib.h>
#include <stdio.h>

#include "ogf_chunks.h"
#include "ogf_tex.c"
#include "ogf_verts.c"
#include "ogf_indices.c"
#include "ogf_swis.c"
#include "chunk_io.c"
#include "file_io.c"
#include "types.h"
#include "file_formats.h"
#include "app\app_types.h"

void read_ogf_header(OgfHeader* ogf_header, Application * app, Chunks* chunks, int chunk_index) {

    // Текущее смещение
    uint32_t offset = chunks->chunks[chunk_index].offset;

    // Прочитать версию формата
    ogf_header->format_version = (uint8_t *) (app->file_data->bytes + offset);
    // Пропустить версию формата
    offset += sizeof(uint8_t);

    // Прочитать тип модели
    ogf_header->model_type = (uint8_t *) (app->file_data->bytes + offset);
    // Пропустить тип модели
    offset += sizeof(uint8_t);

    // Прочитать ID шейдера
    ogf_header->shader_id = (uint16_t *) (app->file_data->bytes + offset);
    // Пропустить ID шейдера
    offset += sizeof(uint16_t);

    // Размер float
    size_t float_size = sizeof(float);

    // Прочитать bbox

    // min bbox
    ogf_header->bbox_min[0] = (float *) (app->file_data->bytes + offset                   );
    ogf_header->bbox_min[1] = (float *) (app->file_data->bytes + offset + float_size      );
    ogf_header->bbox_min[2] = (float *) (app->file_data->bytes + offset + float_size * 2  );

    // max bbox
    ogf_header->bbox_max[0] = (float *) (app->file_data->bytes + offset + float_size * 3  );
    ogf_header->bbox_max[1] = (float *) (app->file_data->bytes + offset + float_size * 4  );
    ogf_header->bbox_max[2] = (float *) (app->file_data->bytes + offset + float_size * 5  );

    // Пропустить bbox
    offset += float_size * 6;

    // Прочитать bsphere

    // bsphere center
    ogf_header->bsphere_center[0] = (float *) (app->file_data->bytes + offset                   );
    ogf_header->bsphere_center[1] = (float *) (app->file_data->bytes + offset + float_size      );
    ogf_header->bsphere_center[2] = (float *) (app->file_data->bytes + offset + float_size * 2  );

    // bsphere radius
    ogf_header->bsphere_radius    = (float *) (app->file_data->bytes + offset + float_size * 3  );

    // Пропустить bsphere
    offset += float_size * 4;

}

void read_ogf_child(OgfChild* ogf_child, Application* app, Chunks* chunks, int chunk_index) {

    Chunks* chunks_child = read_chunks(
        app->file_data,
        chunks->chunks[chunk_index].offset,
        chunks->chunks[chunk_index].offset + *chunks->chunks[chunk_index].size
    );

    for (int i=0; i<chunks_child->count; i++) {

        if (*chunks_child->chunks[i].id == HEADER) {
            read_ogf_header(&ogf_child->header, app, chunks_child, i);

        } else if (*chunks_child->chunks[i].id == TEXTURE) {
            read_texture(ogf_child, app, &chunks_child->chunks[i]);

        } else if (*chunks_child->chunks[i].id == VERTICES) {
            read_verts(ogf_child, app->file_data, &chunks_child->chunks[i]);

        } else if (*chunks_child->chunks[i].id == INDICES) {
            read_indices(ogf_child, app->file_data, &chunks_child->chunks[i]);

        } else if (*chunks_child->chunks[i].id == SWIDATA) {
            read_swis(ogf_child, app->file_data, &chunks_child->chunks[i]);
        }

    }

}

void read_ogf_children(OgfFile* ogf_file, Application* app, Chunks* chunks, int chunk_index) {
    Chunks* chunks_children = read_chunks(
        app->file_data,
        chunks->chunks[chunk_index].offset,
        chunks->chunks[chunk_index].offset + *chunks->chunks[chunk_index].size
    );

    ogf_file->children          = (OgfChild*) malloc(sizeof(OgfChild) * chunks_children->count);
    ogf_file->children_count    = 0;

    for (int visual_id=0; visual_id<chunks_children->count; visual_id++) {
        read_ogf_child(&ogf_file->children[visual_id], app, chunks_children, visual_id);
        ogf_file->children_count++;
    }

}

// освободить память
void free_ogf_file(OgfFile* ogf_file) {
    if (ogf_file != NULL) {
        if (ogf_file->children != NULL) {
            free(ogf_file->children);
        }
        free(ogf_file);
    }
}

OgfFile* read_ogf(Application * app, Chunks* chunks) {

    OgfFile* ogf_file = malloc(sizeof(OgfFile));

    for (int i=0; i<chunks->count; i++) {

        if (*chunks->chunks[i].id == (uint32_t)HEADER) {
            read_ogf_header(&ogf_file->header, app, chunks, i);

        } else if (*chunks->chunks[i].id == (uint32_t)CHILDREN) {
            read_ogf_children(ogf_file, app, chunks, i);
        }

    }

    return ogf_file;

}

void read_ogf_file(Application * app, char* argv[]) {

    // Путь к файлу
    app->file_path = argv[1];

    // Прочитать данные файла
    app->file_data = read_file(app->file_path);

    // Создать структуру для чанков ogf файла
    Chunks* chunks = NULL;

    // Прочитать чанки
    chunks = read_chunks(app->file_data, 0, (uint32_t)app->file_data->size);

    // Прочитать данные ogf файла
    app->file = read_ogf(app, chunks);

    // Освободить память чанков
    free_chunks(chunks);

    // Установить тип файла в ogf
    app->file_type = FILE_OGF;

}

#endif    // OGF_IO_C
