#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <cstdint>
#include <string>
#include <list>
#pragma comment(lib,"ws2_32.lib")

class Window;

using int64 = std::int64_t;
using int32 = std::int32_t;
using int16 = std::int16_t;
using int8 = std::int8_t;
using uInt64 = std::uint64_t;
using uInt32 = std::uint32_t;
using uInt16 = std::uint16_t;
using uInt8 = std::uint8_t;
#define PACKET_BUFFER_SIZE 2048
#define PACKET_HEADER_SIZE 4

extern HWND g_hWindowHandle;
extern RECT g_clientRect;
extern Window* g_pWindow;