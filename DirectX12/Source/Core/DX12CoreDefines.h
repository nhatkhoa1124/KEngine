#pragma once

#ifdef BUILD_DLL
#define KRENDER_API __declspec(dllexport)
#else
#define KRENDER_API __declspec(dllimport)
#endif

#define CLIENT_WIDTH 1280
#define CLIENT_HEIGHT 720

#define VERTEX_SHADER_TARGET "vs_5_0"
#define PIXEL_SHADER_TARGET "ps_5_0"
