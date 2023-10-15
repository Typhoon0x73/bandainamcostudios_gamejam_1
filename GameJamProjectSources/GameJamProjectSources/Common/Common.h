#pragma once
#ifndef BNSCUP_COMMON_H_
#define BNSCUP_COMMON_H_

#ifdef _DEBUG

#define DEBUG_BREAK(x) if((x)) __debugbreak()

#else // _DEBUG

#define DEBUG_BREAK(x) ((void)x)

#endif // _DEBUG

namespace bnscup
{

}

#endif // !BNSCUP_COMMON_H_
