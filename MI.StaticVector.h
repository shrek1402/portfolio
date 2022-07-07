#pragma once

#include <algorithm>
#include <array>

#include "Common/MI.Check.h"
#include "Common/MI.GetUnwrapped.h"
#include "Common/MI.Hash.h"
#include "Common/MI.If.h"
#include "Common/MI.IsIterator.h"
#include "Common/MI.VerifyRange.h"
#include "MI.Property.h"

namespace mi {
template<class, size_t>
class static_vector;

template<class Ty, size_t Size>
class static_vector {
  public:
    using static_capacity = STD integral_constant<size_t, Size>;

  public:
    using container_type = STD array<Ty, Size>;

  public:
    using value_type = typename container_type::value_type;

    using size_type       = typename container_type::size_type;
    using difference_type = typename container_type::difference_type;

    using pointer       = typename container_type::pointer;
    using const_pointer = typename container_type::const_pointer;

    using reference       = typename container_type::reference;
    using const_reference = typename container_type::const_reference;

    using iterator               = typename container_type::iterator;
    using const_iterator         = typename container_type::const_iterator;
    using reverse_iterator       = typename container_type::reverse_iterator;
    using const_reverse_iterator = typename container_type::const_reverse_iterator;

  private:
    MI_CONSTEXPR_17 void _tidy() noexcept {
      _container = container_type{};
      _size      = size_type{0};
    }

  public:
    // Только default, иначе не constexpr.
    ~static_vector() = default;

  public:
    MI_CONSTEXPR_17 static_vector()
        : _container(container_type{}),
          _size(size_type{0}) {
    }

  public:
    MI_CONSTEXPR_17 static_vector(const static_vector& lv_other)
        : _container(lv_other._container),
          _size(lv_other._size) {
    }

    MI_CONSTEXPR_17 static_vector& operator=(const static_vector& lv_other) {
      if (this != STD addressof(lv_other)) {
        _container = lv_other._container;
        _size      = lv_other._size;
      }

      return *this;
    }

  public:
    MI_CONSTEXPR_17 static_vector(static_vector&& rv_other) noexcept
        : _container(STD move(rv_other._container)),
          _size(STD move(rv_other.size())) {
      rv_other._container = {};
      rv_other._size      = {};
    }

    MI_CONSTEXPR_17 static_vector& operator=(static_vector&& rv_other) noexcept {
      _container = STD move(rv_other._container);
      _size      = STD move(rv_other._size);

      rv_other._container = {};
      rv_other._size      = {};

      return *this;
    }

  public:
    explicit static_vector(const size_type count) {
      STD fill(begin(), begin() + count, value_type{});
      _size = count;
    }

    static_vector(const size_type count, const value_type& value) {
      STD fill(begin(), end(), value);
      _size = count;
    }

  public:
    MI_CONSTEXPR_17 static_vector(STD initializer_list<value_type> list)
        : _size(size_type{0}) {
      MI_CHECK(list.size() <= static_capacity::value);

      for (const auto& element: list) {
        emplace_back(element);
      }
    }

    MI_CONSTEXPR_17 static_vector& operator=(STD initializer_list<value_type> list) {
      MI_CHECK(list.size() <= static_capacity::value);

      for (const auto& element: list) {
        emplace_back(element);
      }

      return *this;
    }

  public:
    template<class ItTy, if_t<is_iterator_v<ItTy>> = 0>
    MI_CONSTEXPR_17 static_vector(ItTy first, ItTy last)
        : _size(size_type{0}) {
      MI verify_range(first, last);

      auto       first_unwrapped = MI get_unwrapped(first);
      const auto last_unwrapped  = MI get_unwrapped(last);

      for (; first_unwrapped != last_unwrapped; ++first_unwrapped) {
        emplace_back(*first_unwrapped);
      }
    }

  public:
    MI_NODISCARD MI_CONSTEXPR_17 size_type size() const {
      return _size;
    }

    MI_NODISCARD MI_CONSTEXPR_17 static size_type max_size() {
      return static_capacity::value;
    }

    MI_NODISCARD MI_CONSTEXPR_17 bool empty() const {
      return _size == 0;
    }

    MI_NODISCARD MI_CONSTEXPR_17 bool full() const {
      return _size == max_size();
    }

  public:
    void fill(const value_type& value) {
      _container.fill(value);
      _size = max_size();
    }

    void swap(static_vector& other) noexcept(noexcept(STD is_nothrow_swappable_v<container_type>)) {
      STD swap(_container, other._container);
      STD swap(_size, other._size);
    }

  public:
    MI_NODISCARD MI_CONSTEXPR_17 const_iterator begin() const {
      return (const_iterator{_container.begin()});
    }

    MI_NODISCARD MI_CONSTEXPR_17 iterator begin() {
      return (iterator{_container.begin()});
    }

  public:
    MI_NODISCARD MI_CONSTEXPR_17 const_iterator end() const {
      return (STD next(_container.begin(), static_cast<typename iterator::difference_type>(_size)));
    }

    MI_NODISCARD MI_CONSTEXPR_17 iterator end() {
      return (STD next(_container.begin(), static_cast<typename iterator::difference_type>(_size)));
    }

  public:
    MI_NODISCARD MI_CONSTEXPR_17 const_reverse_iterator rbegin() const {
      return (const_reverse_iterator{_container.end()});
    }

    MI_NODISCARD MI_CONSTEXPR_17 reverse_iterator rbegin() {
      return (reverse_iterator{_container.end()});
    }

  public:
    MI_NODISCARD MI_CONSTEXPR_17 const_reverse_iterator rend() const {
      return (const_reverse_iterator{_container.begin()});
    }

    MI_NODISCARD MI_CONSTEXPR_17 reverse_iterator rend() {
      return (reverse_iterator{_container.begin()});
    }

  public:
    MI_NODISCARD MI_CONSTEXPR_17 const_reverse_iterator crbegin() const {
      return (const_reverse_iterator{_container.begin()});
    }

    MI_NODISCARD MI_CONSTEXPR_17 const_iterator cbegin() {
      return (const_iterator{_container.begin()});
    }

  public:
    MI_NODISCARD MI_CONSTEXPR_17 const_iterator cend() const {
      return (const_iterator{_container.end()});
    }

    MI_NODISCARD MI_CONSTEXPR_17 const_reverse_iterator crend() const {
      return (const_reverse_iterator{_container.end()});
    }

  public:
    MI_CONSTEXPR_17 pointer unchecked_begin() {
      return STD addressof(*begin());
    }

    MI_CONSTEXPR_17 const_pointer unchecked_begin() const {
      return STD addressof(*begin());
    }

  public:
    MI_CONSTEXPR_17 pointer unchecked_end() {
      return STD addressof(*end());
    }

    MI_CONSTEXPR_17 const_pointer unchecked_end() const {
      return STD addressof(*end());
    }

  public:
    MI_NODISCARD MI_CONSTEXPR_17 reference at(size_type pos) {
      MI_CHECK(pos < size());

      return _container[pos];
    }

    MI_NODISCARD MI_CONSTEXPR_17 const_reference at(size_type pos) const {
      MI_CHECK(pos < size());

      return _container[pos];
    }

  public:
    MI_NODISCARD MI_CONSTEXPR_17 reference operator[](size_type pos) {
      MI_DCHECK(pos < _size);

      return _container[pos];
    }

    MI_NODISCARD MI_CONSTEXPR_17 const_reference operator[](size_type pos) const {
      MI_DCHECK(pos < _size);

      return _container[pos];
    }

  public:
    MI_NODISCARD MI_CONSTEXPR_17 reference front() {
      return _container[0];
    }

    MI_NODISCARD MI_CONSTEXPR_17 const_reference front() const {
      return _container[0];
    }

  public:
    MI_NODISCARD MI_CONSTEXPR_17 reference back() {
      return _container[_size - 1];
    }

    MI_NODISCARD MI_CONSTEXPR_17 const_reference back() const {
      return _container[_size - 1];
    }

  public:
    MI_NODISCARD MI_CONSTEXPR_17 pointer data() {
      return _container.data();
    }

    MI_NODISCARD MI_CONSTEXPR_17 const_pointer data() const {
      return _container.data();
    }

  public:
    MI_NODISCARD friend bool operator==(const static_vector& lhs, const static_vector& rhs) {
      return STD equal_to<container_type>{}(lhs._container, rhs._container);
    }

    MI_NODISCARD friend bool operator!=(const static_vector& lhs, const static_vector& rhs) {
      return !(lhs == rhs);
    }

    MI_NODISCARD friend bool operator<(const static_vector& lhs, const static_vector& rhs) {
      return STD less<container_type>{}(lhs._container, rhs._container);
    }

    MI_NODISCARD friend bool operator>(const static_vector& lhs, const static_vector& rhs) {
      return rhs < lhs;
    }

    MI_NODISCARD friend bool operator<=(const static_vector& lhs, const static_vector& rhs) {
      return !(lhs > rhs);
    }

    MI_NODISCARD friend bool operator>=(const static_vector& lhs, const static_vector& rhs) {
      return !(lhs < rhs);
    }

  public:
    template<class... TyVal>
    MI_CONSTEXPR_17 void emplace_back(TyVal&&... values) {
      MI_CHECK(!full());

      _container[_size] = {STD forward<TyVal>(values)...};
      ++_size;
    }

    template<class... TyVal>
    MI_CONSTEXPR_17 void push_back(TyVal&&... values) {
      emplace_back(STD forward<TyVal>(values)...);
    }

  public:
    MI_NODISCARD MI_CONSTEXPR_17 bool contains(const Ty& value) const {
      return STD find(begin(), end(), value) != end();
    }

  public:
    MI_CONSTEXPR_17 void resize(const size_type new_size) {
      if (new_size > _size) {
        const size_type diff_size = new_size - _size;

        for (size_t i = 0; i < diff_size; ++i) {
          emplace_back(value_type{});
        }
      } else {
        const size_type prev_size = _size;

        _size = new_size;

        for (size_type i = _size; i < prev_size; ++i) {
          this->operator[](i) = value_type{};
        }
      }
    }

  public:
    MI_CONSTEXPR_17 void clear() {
      _tidy();
    }

  private:
    container_type _container;  // Контейнер
    size_type      _size;       // Текущий размер массива
};
}  // namespace mi

template<class Ty, size_t Size>
struct mi::hash<MI static_vector<Ty, Size>> {
    STD size_t operator()(const static_vector<Ty, Size>& s) const {
      STD size_t seed(0);

      for (const auto& val: s) {
        const STD size_t hash_val = MI hash<Ty>{}(val);
        seed                      = MI hash_combine(seed, hash_val);
      }

      return seed;
    }
};
