#pragma once
static bool running = true;



bool createWindow(int width, int height, char* title);
void updateWindow();

void* platform_load_gl_function(char* functionPointer);
void platform_swap_buffers();
void* loadDynamicLibrary(char* dll);
void* loadDynamicFunction(void* dll, char* fn);
bool freeDLL(void* dll);
void platform_fill_keycode_lookup_table();
