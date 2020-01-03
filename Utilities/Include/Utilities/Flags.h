#pragma once
#ifndef _UTILZ_FLAGS_H_
#define _UTILZ_FLAGS_H_

#ifndef ENUM_FLAG_OPERATOR
#define ENUM_FLAG_OPERATOR(T,X) inline T operator X (const T& lhs, const T& rhs) { return (T) (static_cast<std::underlying_type_t <T>>(lhs) X static_cast<std::underlying_type_t <T>>(rhs)); } 
#endif
#ifndef ENUM_FLAG_OPERATOR2
#define ENUM_FLAG_OPERATOR2(T,X) inline void operator X= ( T& lhs,const T& rhs) { lhs = (T)(static_cast<std::underlying_type_t <T>>(lhs) X static_cast<std::underlying_type_t <T>>(rhs)); } 
#endif
#ifndef ENUM_FLAGS
#define ENUM_FLAGS(T) \
inline T operator~ (const T& t) { return (T) (~static_cast<std::underlying_type_t <T>>(t)); } \
inline bool flag_has (const T& lhs, const T& rhs) { return (static_cast<std::underlying_type_t <T>>(lhs) & static_cast<std::underlying_type_t <T>>(rhs));  } \
ENUM_FLAG_OPERATOR2(T,|) \
ENUM_FLAG_OPERATOR2(T,&) \
ENUM_FLAG_OPERATOR(T,|) \
ENUM_FLAG_OPERATOR(T,&) \
ENUM_FLAG_OPERATOR(T,^)
#endif
//enum class T
//ENUM_FLAG_OPERATOR(T,&)

#endif