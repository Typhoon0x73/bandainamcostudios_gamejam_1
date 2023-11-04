#pragma once
#ifndef BNSCUP_COMMON_H_
#define BNSCUP_COMMON_H_

#ifdef _DEBUG

#define DEBUG_BREAK(x) if((x)) __debugbreak()

#else // _DEBUG

#define DEBUG_BREAK(x) ((void)(x))

#endif // _DEBUG

namespace bnscup
{
	constexpr unsigned long WINDOW_SIZE_W = 1280;
	constexpr unsigned long WINDOW_SIZE_H =  960;

	constexpr const char32_t* GAME_TITLE = U"きゅうじょたい";
}

#endif // !BNSCUP_COMMON_H_
