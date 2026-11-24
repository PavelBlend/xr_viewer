#ifndef LEVEL_TYPES_H
#define LEVEL_TYPES_H

#define SUPPORTED_LEVEL_VERSION 14

typedef struct LevelHeader {
    uint16_t *      format_version;
    uint16_t *      quality;
} LevelHeader;

typedef struct LevelFile {
    LevelHeader     header;
} LevelFile;

#endif    // LEVEL_TYPES_H
