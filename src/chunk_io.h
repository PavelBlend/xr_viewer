#ifndef CHUNK_IO_H
#define CHUNK_IO_H

typedef struct Chunk {
    uint32_t* id;
    uint32_t* size;
    uint32_t offset;
} Chunk;

typedef struct Chunks {
    uint32_t count;
    Chunk* chunks;
} Chunks;

void        free_chunks         (Chunks*   chunked_file                                     );
uint32_t    get_chunks_count    (FileData* file_data,   uint32_t start,     uint32_t end    );
Chunks*     read_chunks         (FileData* file_data,   uint32_t start,     uint32_t end    );

#endif    // CHUNK_IO_H
