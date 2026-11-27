#pragma once

typedef struct Stats {

    // Количество кадров в секунду
    double fps;

    // Последнее время для подсчёта FPS
    double last_time;

    // Общее количество вершин
    uint32_t total_vertices;

    // Общее количество индексов
    uint32_t total_indices;

} Stats;
