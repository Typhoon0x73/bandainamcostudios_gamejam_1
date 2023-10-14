#pragma once
#ifndef BNCUP_COMMON_H_
#define BNCUP_COMMON_H_

#ifdef _DEBUG

#define DEBUG_BREAK(x) if((x)) __debugbreak()

#else // _DEBUG

#define DEBUG_BREAK(x) ((void)x)

#endif // _DEBUG

namespace bncup
{

}

#endif // !BNCUP_COMMON_H_
