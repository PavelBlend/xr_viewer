#pragma once

#include "..\types.h"
#include "..\chunk_io.h"

// прочитать gcontainer
void read_ogf_gcontainer(GContainer* gcontainer, Application * app, Chunks* chunks, uint32_t chunk_id) {

    // Текущее смещение
    uint32_t offset = chunks->chunks[chunk_id].offset;

    // Прочитать VB данные
    gcontainer->vb_index = (uint32_t *) (app->file_data->bytes + offset);
    offset += sizeof(uint32_t);

    gcontainer->vb_offset = (uint32_t *) (app->file_data->bytes + offset);
    offset += sizeof(uint32_t);

    gcontainer->vb_size = (uint32_t *) (app->file_data->bytes + offset);
    offset += sizeof(uint32_t);

    // Прочитать IB данные
    gcontainer->ib_index = (uint32_t *) (app->file_data->bytes + offset);
    offset += sizeof(uint32_t);

    gcontainer->ib_offset = (uint32_t *) (app->file_data->bytes + offset);
    offset += sizeof(uint32_t);

    gcontainer->ib_size = (uint32_t *) (app->file_data->bytes + offset);
    offset += sizeof(uint32_t);

}
