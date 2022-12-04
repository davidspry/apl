//
// Created by David Spry on 1/10/2022.
//

#pragma once

#include <apl/types/detail/array_detail.hpp>
#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <span>
#include <tuple>
#include <type_traits>
#include <utility>

namespace apl {

//! @brief A statically-allocated array.
//! @tparam value_type The underlying value type.
//! @tparam NUMBER_OF_ELEMENTS The number of elements to allocate.

template<typename value_type, std::size_t NUMBER_OF_ELEMENTS> requires (NUMBER_OF_ELEMENTS > 0)
class array final {
private:
    using array_type = std::array<value_type, NUMBER_OF_ELEMENTS>;

    enum class construct_by {
        prefixing,
        repeating
    };

public:
    //! MARK: - Constructors

    constexpr array() requires detail::default_constructable<value_type> = default;
    constexpr array(array&&) noexcept = default;
    constexpr array(array const&) = default;
    constexpr array& operator=(array&&) noexcept = default;
    constexpr array& operator=(array const&) noexcept = default;

    constexpr explicit array(std::array<value_type, NUMBER_OF_ELEMENTS>&& array):
            m_memory(std::move(array)) {
    }

    constexpr explicit array(const std::array<value_type, NUMBER_OF_ELEMENTS>& array):
            m_memory(array) {
    }

    template<typename... value_type_constructor_arguments>
    constexpr explicit array(value_type_constructor_arguments&& ... constructor_arguments):
            m_memory(array_from(std::forward<value_type_constructor_arguments>(constructor_arguments)...)) {
    }

    constexpr explicit array(value_type&& fill_value) requires detail::default_constructable<value_type> &&
                                                               std::copyable<value_type>:
            m_memory(array_from(std::move(fill_value))) {
    }

    constexpr explicit array(const value_type& fill_value) requires detail::default_constructable<value_type> &&
                                                                    std::copyable<value_type>:
            m_memory(array_from(fill_value)) {
    }

    //! MARK: - Prefixing

    template<std::size_t OTHER_ARRAY_SIZE>
    constexpr static array prefixing(array<value_type, OTHER_ARRAY_SIZE>&& array) {
        return apl::array(array_from(std::move(array)));
    }

    template<std::size_t OTHER_ARRAY_SIZE>
    constexpr static array prefixing(const array <value_type, OTHER_ARRAY_SIZE>& array) {
        return apl::array(array_from(array));
    }

    template<std::size_t OTHER_ARRAY_SIZE>
    constexpr static array prefixing(std::array<value_type, OTHER_ARRAY_SIZE>&& array) {
        return apl::array(array_from(std::move(array)));
    }

    template<std::size_t OTHER_ARRAY_SIZE>
    constexpr static array prefixing(const std::array<value_type, OTHER_ARRAY_SIZE>& array) {
        return apl::array(array_from(array));
    }

    template<typename span_value_type>
    requires std::same_as<typename std::remove_const<span_value_type>::type, value_type>
    constexpr static array prefixing(const std::span<span_value_type> span) {
        return apl::array(array_from(span));
    }

    template<detail::decays_to<value_type> value_type_instance, detail::decays_to<value_type>... value_type_instances>
    requires (sizeof...(value_type_instances) < NUMBER_OF_ELEMENTS)
    constexpr static array prefixing(value_type_instance&& fill_value, value_type_instances&& ... fill_values) {
        return apl::array(array_from<construct_by::prefixing>(
                std::forward_as_tuple(std::forward<value_type_instance>(fill_value),
                                      std::forward<value_type_instances>(fill_values)...))
        );
    }

    //! MARK: - Repeating

    template<std::size_t OTHER_ARRAY_SIZE>
    constexpr static array repeating(array<value_type, OTHER_ARRAY_SIZE>&& array) {
        return apl::array(array_from_repeating(std::move(array)));
    }

    template<std::size_t OTHER_ARRAY_SIZE>
    constexpr static array repeating(const array <value_type, OTHER_ARRAY_SIZE>& array) {
        return apl::array(array_from_repeating(array));
    }

    template<std::size_t OTHER_ARRAY_SIZE>
    constexpr static array repeating(std::array<value_type, OTHER_ARRAY_SIZE>&& array) {
        return apl::array(array_from_repeating(std::move(array)));
    }

    template<std::size_t OTHER_ARRAY_SIZE>
    constexpr static array repeating(const std::array<value_type, OTHER_ARRAY_SIZE>& array) {
        return apl::array(array_from_repeating(array));
    }

    template<typename span_value_type>
    requires std::same_as<typename std::remove_const<span_value_type>::type, value_type>
    constexpr static array repeating(const std::span<span_value_type> span) {
        return apl::array(array_from_repeating(span));
    }

    template<detail::decays_to<value_type> value_type_instance, detail::decays_to<value_type>... value_type_instances>
    requires (sizeof...(value_type_instances) < NUMBER_OF_ELEMENTS)
    constexpr static array repeating(value_type_instance&& fill_value, value_type_instances&& ... fill_values) {
        return apl::array(array_from<construct_by::repeating>(
                std::forward_as_tuple(std::forward<value_type_instance>(fill_value),
                                      std::forward<value_type_instances>(fill_values)...))
        );
    }

    //! MARK: - Public Methods

    template<detail::decays_to<value_type> value_type_instance>
    requires std::copyable<value_type>
    constexpr void fill(value_type_instance&& fill_value) {
        m_memory.fill(std::forward<value_type_instance>(fill_value));
    }

    [[nodiscard]]
    constexpr auto size() const -> std::size_t {
        return m_memory.size();
    }

    [[nodiscard]]
    constexpr auto at(const std::size_t index) noexcept(false) -> value_type& {
        return m_memory.at(index);
    }

    [[nodiscard]]
    constexpr auto at(const std::size_t index) const noexcept(false) -> const value_type& {
        return m_memory.at(index);
    }

    //! MARK: - Operators

    [[nodiscard]]
    constexpr auto operator[](const std::size_t index) -> value_type& {
        return m_memory[index];
    }

    [[nodiscard]]
    constexpr auto operator[](const std::size_t index) const -> const value_type& {
        return m_memory[index];
    }

    template<std::size_t OTHER_ARRAY_SIZE>
    constexpr auto operator+(const array<value_type, OTHER_ARRAY_SIZE>& array) const {
        auto new_array = std::array<value_type, NUMBER_OF_ELEMENTS + OTHER_ARRAY_SIZE>{};
        std::copy(m_memory.cbegin(), m_memory.cend(), new_array.begin());
        std::copy(array.begin(), array.end(), new_array.begin() + m_memory.size());
        return apl::array<value_type, NUMBER_OF_ELEMENTS + OTHER_ARRAY_SIZE>(std::move(new_array));
    }

    template<std::size_t OTHER_ARRAY_LENGTH>
    requires (NUMBER_OF_ELEMENTS != OTHER_ARRAY_LENGTH)
    constexpr bool operator==(const array <value_type, OTHER_ARRAY_LENGTH>&) const {
        return false;
    };

    constexpr bool operator==(const array <value_type, NUMBER_OF_ELEMENTS>&) const = default;

    //! MARK: - Iterators

    [[nodiscard]] constexpr auto begin() { return m_memory.begin(); }
    [[nodiscard]] constexpr auto begin() const { return m_memory.begin(); }
    [[nodiscard]] constexpr auto rbegin() { return m_memory.rbegin(); }
    [[nodiscard]] constexpr auto rbegin() const { return m_memory.rbegin(); }

    [[nodiscard]] constexpr auto end() { return m_memory.end(); }
    [[nodiscard]] constexpr auto end() const { return m_memory.end(); }
    [[nodiscard]] constexpr auto rend() { return m_memory.rend(); }
    [[nodiscard]] constexpr auto rend() const { return m_memory.rend(); }

private:
    //! MARK: - Array From

    template<std::size_t... index_sequence, typename... value_type_arguments>
    requires std::copyable<value_type>
    constexpr static auto array_from(std::index_sequence<index_sequence...>&&,
                                     value_type_arguments&& ... arguments) -> array_type {
        const auto initialised_value = value_type(std::forward<value_type_arguments>(arguments)...);
        return {(static_cast<void>(index_sequence), initialised_value)...};
    }

    template<std::size_t... index_sequence, typename... value_type_arguments>
    constexpr static auto array_from(std::index_sequence<index_sequence...>&&,
                                     value_type_arguments&& ... arguments) -> array_type {
        return {(static_cast<void>(index_sequence), value_type(std::forward<value_type_arguments>(arguments)...))...};
    }

    template<typename... argument_types>
    constexpr static auto array_from(argument_types&& ... arguments) {
        return array_from(std::make_index_sequence<NUMBER_OF_ELEMENTS>(),
                          std::forward<argument_types>(arguments)...);
    }

    constexpr static auto array_from(const value_type& fill_value) {
        auto new_array = array_type{};
        new_array.fill(fill_value);
        return new_array;
    }

    constexpr static auto array_from(detail::iterable auto&& array) {
        auto new_array = array_type{};
        const auto array_size = std::size(array);
        const auto items_size = std::min(array_size, NUMBER_OF_ELEMENTS);
        if constexpr (std::is_rvalue_reference<decltype(array)>::value &&
                      std::is_move_constructible<value_type>::value) {
            std::move(std::begin(array), std::begin(array) + items_size, std::begin(new_array));
        } else {
            std::copy_n(std::begin(array), items_size, std::begin(new_array));
        }
        return new_array;
    }

    constexpr static auto array_from_repeating(const detail::iterable auto& array) {
        auto new_array = array_type{};
        if (array.size() > 0) {
            for (std::size_t i = 0; i < NUMBER_OF_ELEMENTS; i += array.size()) {
                const auto elements_to_copy = std::min(array.size(), NUMBER_OF_ELEMENTS - i);
                std::copy(std::begin(array), std::begin(array) + elements_to_copy, std::begin(new_array) + i);
            }
        }
        return new_array;
    }

    template<construct_by pattern, detail::decays_to<value_type> value_type_instance, detail::decays_to<value_type>... value_type_instances>
    constexpr static auto array_from(std::tuple<value_type_instance, value_type_instances...>&& tuple) {
        return std::apply([](value_type_instance&& first_value, value_type_instances&& ... tuple_values) {
            if constexpr (pattern == construct_by::repeating) {
                return array_from_repeating(std::array{std::move(first_value), std::move(tuple_values)...});
            } else {
                return array_from(std::array{std::move(first_value), std::move(tuple_values)...});
            }
        }, std::move(tuple));
    }

    std::array<value_type, NUMBER_OF_ELEMENTS> m_memory;
};

}
