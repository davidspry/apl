# apl

a programming library

---
# array

A set of convenience initialisers for a `std::array`.

## usage

### Initialise from a `std::array`
```cpp
constexpr auto array1 = std::array{1, 2, 3}; //! [1, 2, 3]
constexpr auto array2 = apl::array{array1};  //! [1, 2, 3]
```

### Initialise with a fill-value
```cpp
constexpr auto array = apl::array<float, 4>(4.0f); //! [4.0f, 4.0f, 4.0f, 4.0f]
```

### Initialise with constructor arguments
```cpp
struct object {
  int m_data1;
  int m_data2;
  constexpr object(const int data1,
                   const int data2): m_data1(data1),
                                     m_data2(data2) {}
};

constexpr auto array = apl::array<object, 3>(4, 8); //! [{4, 8}, {4, 8}, {4, 8}]
```

### Initialise by prefixing or repeating
```cpp
//! Initialise from elements
constexpr auto one = 1;
constexpr auto prefix = apl::array<int, 5>::prefixing(one, 2, 3);    //! [1, 2, 3, 0, 0]
constexpr auto repeat = apl::array<int, 5>::repeating(one, 2, 3, 4); //! [1, 2, 3, 4, 1] 
```

```cpp
//! Initialise from a smaller std::array
constexpr auto source = std::array<int, 2>{2, 4};              //! [2, 4]
constexpr auto prefix = apl::array<int, 5>::prefixing(source); //! [2, 4, 0, 0, 0]
constexpr auto repeat = apl::array<int, 5>::repeating(source); //! [2, 4, 2, 4, 2]
```

```cpp
//! Initialise from a larger std::array
constexpr auto source = std::array<int, 6>{1, 2, 3, 4, 5, 6};  //! [1, 2, 3, 4, 5, 6]
constexpr auto prefix = apl::array<int, 5>::prefixing(source); //! [1, 2, 3, 4, 5]
constexpr auto repeat = apl::array<int, 5>::repeating(source); //! [1, 2, 3, 4, 5]
```

```cpp
//! Initialise from a std::span
auto source = std::span(std_vector);                 //! [1, 2, 3]
auto prefix = apl::array<int, 6>::prefixing(source); //! [1, 2, 3, 0, 0, 0]
auto repeat = apl::array<int, 6>::repeating(source); //! [1, 2, 3, 1, 2, 3]
```
---
# mod

A number whose value is computed using the modulo operation with respect to a predefined limit.

## usage
```cpp
constexpr auto limit = 7uz;
auto m = apl::mod<limit>{};

for (auto i = 0; i < 128; ++i) {
    assert((++m).value() < limit);
}
```
---
# mwmr_circular_queue

A circular queue supporting multiple writers and multiple readers.

## usage
```cpp
constexpr auto limit = 64;
using value_type = std::string;
auto mwmr = mwmr_circular_queue<value_type, limit>{};

for (auto i = 0; i < limit; ++i) {
    const auto did_push = mwmr.push(
            fmt::format("{}", i)
    );
    
    assert(did_push);
}

for (auto i = 0; i < limit; ++i) {
    const auto opt_value = mwmr.pop();
    
    assert(
        opt_value.has_value() &&
        opt_value.value() == fmt::format("{}", i)
    );
}
```
---
# range

An iterable, half-open range over the integers.

## usage
```cpp
for (auto i: apl::range<0, 5>{}) {
    //! [0, 1, 2, 3, 4]
}

for (auto i: apl::range<0, 5>::reversed{}) {
    //! [4, 3, 2, 1, 0]
}

for (auto i: apl::range<0, 9>::step_by<3>{}) {
    //! [0, 3, 6]
}

for (auto i: apl::range<0, 5>::drop_prefix<3>::reversed{}) {
    //! [4, 3]
}
```
---
# seqlock

A thread-safe object-wrapper based on the 'seqlock' spin-lock algorithm, which supports multiple writers and multiple readers.

## usage
```cpp
auto locked = apl::seqlock<std::string_view>{};
locked.store("value");
assert(
    locked.load() == std::string_view("value")
);
```
---
# thread group

A group of threads that each execute pending tasks.

## usage
```cpp
//! Four threads, 125ms wait-interval between units of work.
auto thread_group = apl::thread_group<4, 125>{};
auto atomic_count = std::atomic<std::size_t>{0};

for (auto i = 0; i < n; ++i) {
    thread_group.submit_task([&atomic_value]() {
        atomic_count.fetch_add(1);
    });
}

while (thread_group.is_busy()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

assert(atomic_count.load() == n);
``` 
