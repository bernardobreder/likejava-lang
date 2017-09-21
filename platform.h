#ifndef LNG_PLATFORM_H
#define LNG_PLATFORM_H

#if UINTPTR_MAX == 0xffffffff
#ifndef __x86_32__
#define __x86_32__
#endif
#elif UINTPTR_MAX == 0xffffffffffffffff
#ifndef __x86_64__
#define __x86_64__
#endif
#endif

#ifdef __x86_64__
#define x686
#else
#define x386
#endif

typedef unsigned char err;
typedef unsigned char byte;
//typedef unsigned char bool;
typedef short int16;
typedef unsigned short uint16;
typedef char int8;
typedef unsigned char uint8;
#ifdef x686
typedef int int32;
typedef long long int64;
typedef unsigned int uint32;
typedef unsigned long uint64;
typedef long int36;
typedef unsigned long uint36;
#else
typedef int int32;
typedef long long int64;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef int int36;
typedef unsigned int uint36;
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
#undef __LINUX__
#define __LINUX__   1
#endif

#if defined(__APPLE__)
#undef __MACOSX__
#define __MACOSX__  1
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
#undef __WIN32__
#define __WIN32__   1
#endif

#endif
