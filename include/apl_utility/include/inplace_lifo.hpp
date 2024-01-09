//! apl
//! David Spry
//! Created on 12/9/2023

#pragma once

#include <stack>

#include <apl_utility/include/inplace_vector.hpp>

namespace apl {

//! @brief A LIFO with a fixed capacity, using in-built storage.

template<class T, std::size_t CAPACITY> requires (CAPACITY > 0)
class inplace_lifo final: public std::stack<T, apl::inplace_vector<T, CAPACITY>> {
  using stack_t = std::stack<T, inplace_vector<T, CAPACITY>>;

public:
  [[nodiscard]] auto full() const -> bool {
    return stack_t::size() == CAPACITY;
  }
};

}
