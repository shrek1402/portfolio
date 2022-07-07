#pragma once

#include "Common/MI.Hash.h"
#include "Common/MI.If.h"
#include "Common/MI.NarrowingCast.h"
#include "Common/MI.TemplateUtil.h"

// Генерирует имя тега из имени алиаса.
#define MI_GENERATE_TAG_NAME(name_alias) name_alias##_tag

// Объявляет тег.
#define MI_DEFINE_TAG_FOR_STRONG_ALIAS(name_alias)                                                                     \
  namespace internal::impl {                                                                                           \
  class MI_GENERATE_TAG_NAME(name_alias) {};                                                                           \
  } /* namespace internal::impl */

// Объявляет новый strong_alias.
#define MI_NEW_STRONG_ALIAS(name_alias, underlying_type)                                                               \
  MI_DEFINE_TAG_FOR_STRONG_ALIAS(name_alias);                                                                          \
                                                                                                                       \
  using name_alias = MI internal::strong_alias<internal::impl::MI_GENERATE_TAG_NAME(name_alias), underlying_type> /*;*/

namespace mi::internal {
// A type-safe alternative for a typedef or a 'using' directive.
//
// C++ currently does not support type-safe typedefs, despite multiple proposals
// (ex. http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3515.pdf). The
// next best thing is to try and emulate them in library code.
//
// The motivation is to disallow several classes of errors:
//
// using Orange = int;
// using Apple = int;
// Apple apple(2);
// Orange orange = apple;      // Orange should not be able to become an Apple.
// Orange x = orange + apple;  // Shouldn't add Oranges and Apples.
// if (orange > apple);        // Shouldn't compare Apples to Oranges.
// void foo(Orange);
// void foo(Apple);            // Redefinition.
// etc.
//
// StrongAlias may instead be used as follows:
//
// using Orange = StrongAlias<class OrangeTag, int>;
// using Apple = StrongAlias<class AppleTag, int>;
// using Banana = StrongAlias<class BananaTag, std::string>;
// Apple apple(2);
// Banana banana("Hello");
// Orange orange = apple;                              // Does not compile.
// Orange other_orange = orange;                       // Compiles, types match.
// Orange x = orange + apple;                          // Does not compile.
// Orange y = Orange(orange.value() + apple.value());  // Compiles.
// Orange z = Orange(banana->size() + *other_orange);  // Compiles.
// if (orange > apple);                                // Does not compile.
// if (orange > other_orange);                         // Compiles.
// void foo(Orange);
// void foo(Apple);                                    // Compiles into separate overload.
//
// StrongAlias is a zero-cost abstraction, it's compiled away.
//
// TagType is an empty tag class (also called "phantom type") that only serves
// the type system to differentiate between different instantiations of the
// template.
template<class TagType, class UnderlyingType>
class strong_alias {
  public:
    using underlying_type = UnderlyingType;
    using value_type      = underlying_type;

  public:
    ~strong_alias() = default;

  public:
    constexpr strong_alias() = default;

  public:
    constexpr strong_alias(const strong_alias& other)     = default;
    constexpr strong_alias(strong_alias&& other) noexcept = default;

  public:
    constexpr strong_alias& operator=(const strong_alias& other)     = default;
    constexpr strong_alias& operator=(strong_alias&& other) noexcept = default;

  public:
    constexpr explicit strong_alias(const underlying_type& val) noexcept
        : _value(val) {
    }

    constexpr explicit strong_alias(underlying_type&& val) noexcept
        : _value(STD move(val)) {
    }

  public:
    constexpr strong_alias& operator=(const underlying_type& val) noexcept {
      _value = val;

      return *this;
    }

    constexpr strong_alias& operator=(underlying_type&& val) noexcept {
      _value = STD move(val);

      return *this;
    }

  public:
    MI_NODISCARD constexpr underlying_type* operator->() {
      return STD addressof(_value);
    }

    MI_NODISCARD constexpr const underlying_type* operator->() const {
      return STD addressof(_value);
    }

  public:
    MI_NODISCARD constexpr underlying_type& operator*() & {
      return _value;
    }

    MI_NODISCARD constexpr const underlying_type& operator*() const& {
      return _value;
    }

    MI_NODISCARD constexpr underlying_type&& operator*() && {
      return STD move(_value);
    }

    MI_NODISCARD constexpr const underlying_type&& operator*() const&& {
      return STD move(_value);
    }

  public:
    MI_NODISCARD constexpr underlying_type& value() & {
      return _value;
    }

    MI_NODISCARD constexpr const underlying_type& value() const& {
      return _value;
    }

    MI_NODISCARD constexpr underlying_type&& value() && {
      return STD move(_value);
    }

    MI_NODISCARD constexpr const underlying_type&& value() const&& {
      return STD move(_value);
    }

  public:
    constexpr explicit operator const underlying_type&() const {
      return (_value);
    }

  public:
    constexpr strong_alias& operator++() {
      ++_value;

      return *this;
    }

    MI_NODISCARD constexpr strong_alias operator++(int) {
      const strong_alias old = *this;
      ++*this;

      return old;
    }

    constexpr strong_alias& operator--() {
      --_value;

      return *this;
    }

    MI_NODISCARD constexpr strong_alias operator--(int) {
      const strong_alias old = *this;
      --*this;

      return old;
    }

    constexpr strong_alias& operator+=(const underlying_type& i) {
      _value += i;

      return *this;
    }

    MI_NODISCARD constexpr strong_alias operator+(const underlying_type& i) const {
      strong_alias it = *this;
      it += i;

      return it;
    }

    constexpr strong_alias& operator-=(const underlying_type& i) {
      _value -= i;

      return *this;
    }

    MI_NODISCARD constexpr strong_alias operator-(const underlying_type& i) const {
      strong_alias it = *this;
      it -= i;

      return it;
    }

  public:
    MI_NODISCARD constexpr bool operator==(const strong_alias& rhs) const {
      return this->_value == rhs._value;
    }

    MI_NODISCARD constexpr bool operator!=(const strong_alias& rhs) const {
      return !(*this == rhs);
    }

    MI_NODISCARD constexpr bool operator<(const strong_alias& rhs) const {
      return this->_value < rhs._value;
    }

    MI_NODISCARD constexpr bool operator>(const strong_alias& rhs) const {
      return rhs < *this;
    }

    MI_NODISCARD constexpr bool operator<=(const strong_alias& rhs) const {
      return !(*this > rhs);
    }

    MI_NODISCARD constexpr bool operator>=(const strong_alias& rhs) const {
      return !(*this < rhs);
    }

  private:
    underlying_type _value;
};

template<class Tag, class Underlying, MI if_t<MI internal::supports_ostream_operator<Underlying>::value> = 0>
constexpr STD ostream& operator<<(STD ostream& str, const strong_alias<Tag, Underlying>& descriptor) {
  str << descriptor.value();

  return str;
}
}  // namespace mi::internal

template<class Tag, class Underlying>
struct MI hash<MI internal::strong_alias<Tag, Underlying>> {
    MI_NODISCARD size_t operator()(const MI internal::strong_alias<Tag, Underlying>& alias) const {
      return hash<Underlying>{}(alias.value());
    }
};
