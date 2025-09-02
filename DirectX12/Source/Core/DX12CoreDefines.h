#pragma once

#ifdef BUILD_DLL
#define KRENDER_API __declspec(dllexport)
#else
#define KRENDER_API __declspec(dllimport)
#endif
