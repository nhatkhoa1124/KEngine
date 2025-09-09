#pragma once

#ifdef BUILD_DLL
#define KRENDER_API __declspec(dllexport)
#else
#define KRENDER_API __declspec(dllimport)
#endif

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
