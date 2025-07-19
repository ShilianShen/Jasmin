#include "lopo.h"


SDL_GPUDevice* device;
SDL_GPUShader* shader;


void LOPO_Init() {
    // load device
    device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_MSL, false, NULL);
    // Req Condition
    if (device == NULL) {
        printf("%s: failed to create GPUDevice, %s.\n", __func__, SDL_GetError());
        return;
    }

    // load shader
    // get code, code_size
    size_t code_size;
    const char* file = "/Users/shilianshen/Documents/Oolong Press/Lo-Tri/src/jasmin/lopo/shaders/debugShader.glsl";
    void *code = SDL_LoadFile(file, &code_size); // malloc
    if (code == NULL) {
        printf("%s: failed to load testShader.hlsl, %s.\n", __func__, SDL_GetError());
    }

    const SDL_GPUShaderCreateInfo info = {
        .code = code,
        .code_size = code_size,
        .entrypoint = "VS",
        .format = SDL_GPU_SHADERFORMAT_MSL,
        .stage = SDL_GPU_SHADERSTAGE_VERTEX,
        .num_samplers = 1,
        .num_storage_textures = 1,
        .num_storage_buffers = 1,
        .num_uniform_buffers = 1,
        .props = 0,
    };
    shader = SDL_CreateGPUShader(device, &info);
    SDL_free(code); // free
    if (shader == NULL) {
        printf("%s: failed to create GPUShader, %s.\n", __func__, SDL_GetError());
        return;
    }
}
void LOPO_Deinit() {
    SDL_ReleaseGPUShader(device, shader);
    shader = NULL;
    SDL_DestroyGPUDevice(device);
    device = NULL;
}


void LOPO_Load() {}
void LOPO_Unload() {}

void LOPO_Renew() {}
void LOPO_Draw() {
}