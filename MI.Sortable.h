#pragma once

#include <cstdint>

#include "Container/MI.SortableBase.h"
#include "MI.Property.h"

// Объявление производного типа sortable
// (int, i, 3) -> sortable3i
#define MI_MAKE_TYPEDEFS(Type, TypeSuffix, Size) using sortable##Size##TypeSuffix = MI sortable<Type, Size>

#define MI_MAKE_TYPEDEFS_SIMPLE(Size)                                                                                  \
  template<class Ty>                                                                                                   \
  using sortable##Size = MI sortable<Ty, Size>

// Объявление производных типов разного размера
#define MI_MAKE_TYPEDEFS_ALL_SIZES(Type, TypeSuffix)                                                                   \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 0);                                                                               \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 1);                                                                               \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 2);                                                                               \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 3);                                                                               \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 4);                                                                               \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 5);                                                                               \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 6);                                                                               \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 7);                                                                               \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 8);                                                                               \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 9);                                                                               \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 10);                                                                              \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 11);                                                                              \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 12);                                                                              \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 13);                                                                              \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 14);                                                                              \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 15);                                                                              \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 16);                                                                              \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 17);                                                                              \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 18);                                                                              \
  MI_MAKE_TYPEDEFS(Type, TypeSuffix, 19);

#define MAKE_TYPEDEFS_SIMPLE_ALL_SIZES                                                                                 \
  MI_MAKE_TYPEDEFS_SIMPLE(0);                                                                                          \
  MI_MAKE_TYPEDEFS_SIMPLE(1);                                                                                          \
  MI_MAKE_TYPEDEFS_SIMPLE(2);                                                                                          \
  MI_MAKE_TYPEDEFS_SIMPLE(3);                                                                                          \
  MI_MAKE_TYPEDEFS_SIMPLE(4);                                                                                          \
  MI_MAKE_TYPEDEFS_SIMPLE(5);                                                                                          \
  MI_MAKE_TYPEDEFS_SIMPLE(6);                                                                                          \
  MI_MAKE_TYPEDEFS_SIMPLE(7);                                                                                          \
  MI_MAKE_TYPEDEFS_SIMPLE(8);                                                                                          \
  MI_MAKE_TYPEDEFS_SIMPLE(9);                                                                                          \
  MI_MAKE_TYPEDEFS_SIMPLE(10);                                                                                         \
  MI_MAKE_TYPEDEFS_SIMPLE(11);                                                                                         \
  MI_MAKE_TYPEDEFS_SIMPLE(12);                                                                                         \
  MI_MAKE_TYPEDEFS_SIMPLE(13);                                                                                         \
  MI_MAKE_TYPEDEFS_SIMPLE(14);                                                                                         \
  MI_MAKE_TYPEDEFS_SIMPLE(15);                                                                                         \
  MI_MAKE_TYPEDEFS_SIMPLE(16);                                                                                         \
  MI_MAKE_TYPEDEFS_SIMPLE(17);                                                                                         \
  MI_MAKE_TYPEDEFS_SIMPLE(18);                                                                                         \
  MI_MAKE_TYPEDEFS_SIMPLE(19);

namespace mi {
// Создать полный список простых типов
MAKE_TYPEDEFS_SIMPLE_ALL_SIZES

// Создать полный список всех производных типов sortable
MI_MAKE_TYPEDEFS_ALL_SIZES(bool, b);

MI_MAKE_TYPEDEFS_ALL_SIZES(char, c);
MI_MAKE_TYPEDEFS_ALL_SIZES(signed char, sc);
MI_MAKE_TYPEDEFS_ALL_SIZES(unsigned char, uc);
#if MI_CPP_VERSION == 20
MI_MAKE_TYPEDEFS_ALL_SIZES(char8_t, c8);
#endif
MI_MAKE_TYPEDEFS_ALL_SIZES(char16_t, c16);
MI_MAKE_TYPEDEFS_ALL_SIZES(char32_t, c32);
MI_MAKE_TYPEDEFS_ALL_SIZES(wchar_t, wc);

MI_MAKE_TYPEDEFS_ALL_SIZES(short, s);
MI_MAKE_TYPEDEFS_ALL_SIZES(int, i);
MI_MAKE_TYPEDEFS_ALL_SIZES(long, l);  //-V126
MI_MAKE_TYPEDEFS_ALL_SIZES(long long, ll);

MI_MAKE_TYPEDEFS_ALL_SIZES(unsigned short, us);
MI_MAKE_TYPEDEFS_ALL_SIZES(unsigned int, ui);
MI_MAKE_TYPEDEFS_ALL_SIZES(unsigned long, ul);  //-V126
MI_MAKE_TYPEDEFS_ALL_SIZES(unsigned long long, ull);

MI_MAKE_TYPEDEFS_ALL_SIZES(float, f);
MI_MAKE_TYPEDEFS_ALL_SIZES(double, d);
MI_MAKE_TYPEDEFS_ALL_SIZES(long double, ld);

MI_MAKE_TYPEDEFS_ALL_SIZES(STD size_t, n);
MI_MAKE_TYPEDEFS_ALL_SIZES(STD ptrdiff_t, p);

MI_MAKE_TYPEDEFS_ALL_SIZES(STD int8_t, i8);
MI_MAKE_TYPEDEFS_ALL_SIZES(STD int16_t, i16);
MI_MAKE_TYPEDEFS_ALL_SIZES(STD int32_t, i32);
MI_MAKE_TYPEDEFS_ALL_SIZES(STD int64_t, i64);

MI_MAKE_TYPEDEFS_ALL_SIZES(STD uint8_t, u8);
MI_MAKE_TYPEDEFS_ALL_SIZES(STD uint16_t, u16);
MI_MAKE_TYPEDEFS_ALL_SIZES(STD uint32_t, u32);
MI_MAKE_TYPEDEFS_ALL_SIZES(STD uint64_t, u64);

}  // namespace mi
