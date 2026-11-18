#ifndef SHADERS_C
#define SHADERS_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>
#include <math.h>

#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "cglm\cglm.h"

#include "globals.c"

float fov = 67.5f;
float far_plane = 2000.0f;
float near_plane = 0.1f;

static void create_shader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType, char* file_path) {

    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    const GLchar* p[1];
    p[0] = pShaderText;
    glShaderSource(ShaderObj, 1, p, NULL);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader \"%s\", type %d: '%s'\n", file_path, ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

char* read_shader_file(char* exe_dir, char* file_name) {

    size_t abs_path_length = strlen(exe_dir) + strlen(file_name) + 1;
    char* abs_path = malloc(abs_path_length);

    size_t exe_len = strlen(exe_dir);
    size_t shader_len = strlen(file_name);

    // Копируем первую строку
    memcpy(abs_path, exe_dir, exe_len);

    // Копируем вторую строку сразу после первой
    memcpy(abs_path + exe_len, file_name, shader_len);

    // Добавляем завершающий нулевой символ
    abs_path[exe_len + shader_len] = 0;

    // printf("Load Shader: \"%s\"\n", abs_path);

    // Открыть файл
    FILE *file = fopen(abs_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open shader file '%s'\n", abs_path);
        exit(1);
    }

    // Определить размер файла
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Выделить память для хранения содержимого файла
    char *file_data = (char *) malloc(file_size + 1);
    if (file_data == NULL) {
        printf("Shader Memory Error");
        fclose(file);
        exit(1);
    }

    memset(file_data, 0, file_size + 1);

    // Чтение содержимого файла
    fread(file_data, 1, file_size, file);
    fclose(file);

    file_data[file_size] = 0;

    return file_data;
}

static void set_shader_matrices(GLFWwindow* window, GLuint ShaderProgram) {

    int width, height;
    glfwGetWindowSize(window, &width, &height);    // Получаем ширину и высоту окна

    float aspect;

    // Чтобы избежать деления на ноль, проверяем высоту
    if (height == 0) {
        aspect = 1.0f;  // Соотношение по умолчанию (квадратное окно)
    } else {
        aspect = ((float)width - 350.0f) / (float)height;    // вычитаем ширину боковой панели (минус 350)
    }

    mat4 model;
    vec3 scale = {1.0, 1.0, -1.0};
    // Создаём единичную матрицу
    glm_mat4_identity(model);
    glm_scale(model, scale);

    // направление камеры (spherical coordinates)
    float rad_x = glm_rad(camera_angle_x);
    float rad_y = glm_rad(-camera_angle_y);

    vec3 up;
    if (cosf(rad_x) < 0.0f) {
        // камера перевёрнута
        up[0] = 0.0f;
        up[1] = -1.0f;
        up[2] = 0.0f;
    } else {
        up[0] = 0.0f;
        up[1] = 1.0f;
        up[2] = 0.0f;
    }

    vec3 center = {pan_x, pan_y, pan_z};

    vec3 eye = {
        pan_x + zoom * cosf(rad_x) * sinf(rad_y),
        pan_y + zoom * sinf(rad_x),
        pan_z + zoom * cosf(rad_x) * cosf(rad_y)
    };

    mat4 view;
    glm_lookat(eye, center, up, view);

    // Матриа проекции
    mat4 projection;
    glm_perspective(glm_rad(fov), aspect, near_plane, far_plane, projection);

    GLint modelLoc = glGetUniformLocation(ShaderProgram, "model");
    GLint projLoc = glGetUniformLocation(ShaderProgram, "projection");
    GLint viewLoc = glGetUniformLocation(ShaderProgram, "view");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat*)model);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (const GLfloat*)projection);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const GLfloat*)view);

}

static GLuint compile_shaders(char* exe_dir) {

    // создать шейдерную программу
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        printf("Error creating shader program\n");
        exit(1);
    }

    // прочитать шейдерный код из файлов
    char *vertex_shader   = read_shader_file(exe_dir, "shaders\\vertex_shader.glsl"     );
    char *fragment_shader = read_shader_file(exe_dir, "shaders\\fragment_shader.glsl"   );

    // создать шейдеры
    create_shader(ShaderProgram, vertex_shader,   GL_VERTEX_SHADER,   "shaders\\vertex_shader.glsl"     );
    create_shader(ShaderProgram, fragment_shader, GL_FRAGMENT_SHADER, "shaders\\fragment_shader.glsl"   );

    free(vertex_shader  );
    free(fragment_shader);

    // слинковать шейдерную программу
    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    // проверить шейдерную программу
    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);

    GLuint gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
    assert(gSampler != 0xFFFFFFFF);
    glUniform1i(gSampler, 0);

    return ShaderProgram;

}

// Установить uniform переменные
void set_uniforms(Application * app) {
    app->render_ctx.useShadelessLoc = glGetUniformLocation(app->render_ctx.ShaderProgram, "useShadeless");
    app->render_ctx.useTextureLoc   = glGetUniformLocation(app->render_ctx.ShaderProgram, "useTexture");
    app->render_ctx.solidColorLoc   = glGetUniformLocation(app->render_ctx.ShaderProgram, "solidColor");
    app->render_ctx.lightDirLoc     = glGetUniformLocation(app->render_ctx.ShaderProgram, "lightDir");
}

// Загрузить шейдеры
void load_shaders(Application * app) {

    // Скомпилировать шейдеры
    app->render_ctx.ShaderProgram = compile_shaders(app->exe_dir);

    // Установить uniform переменные
    set_uniforms(app);

}

#endif      // SHADERS_C
