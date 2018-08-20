//
// Created by bentoo on 22.05.18.
//

#ifndef YAGE_DEFINES_H
#define YAGE_DEFINES_H

#if _WIN32
#ifdef YAGE_EXPORTS
#define YAGE_API __declspec(dllexport)
#else
#define YAGE_API __declspec(dllimport)
#endif
#else
#define YAGE_API
#endif

#endif // YAGE_DEFINES_H