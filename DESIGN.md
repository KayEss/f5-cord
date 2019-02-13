# Library design


Both `string` and `string_view` types are centred around two member variables:

* `char const *data` points to the start of the string.
* `std::size_t count` the number of characters.

The main difference between them is in how the pointer is treated. With a `string`, which owns the memory, taking a copy of the string instance requires us to allocate new storage and copy the string content for that new instance. This is because only one instance can own the memory at a time.

With a `string_view` however we can just copy the pointer and the size because the view doesn't own any memory. The down side is that we must understand the life time of where the view's data is actually stored in order to know what we can do with the `string_view`.

The requirement to continually allocate for copies of `std::string` can be quite expensive. `std::string` must even do this for string literals. Although a `std::string_view` can be used, quite safely, for string literals, it is very hard to ensure in later code that the `std::string_view` was created from a literal and not from a `std::string` or some other source. In any case, if we want to put the literal into a `std::string` we must allocate new memory.

Some of these constant allocations can be aleviated by changing the type of pointer in the `string` type. If a `shared_ptr<char>` is used in preference to a raw pointer, then string copies can be made with an interlocked increment.

It also means that sub-string operations can be cheaper, if like `string_view` we forego the `c_str()` member on the `string` type. In effect this means that we move from:

```cpp
struct string_core {
    char const *data;
    std::size_t count;
};
struct string_view_core {
    char const *data;
    std::size_t count;
};
```

To something that looks like this:

```cpp
struct string_core {
    std::shared_ptr<char const> data;
    std::size_t count;
};
struct string_view_core {
    char const *data;
    std::size_t count;
};
```

It turns out that we can something a little bit more interesting though. The `shared_ptr<char>` is in fact a "fat" pointer. It needs to point to both the `char` data for this string instance, and a control block that has the reference counts and the originally allocated memory pointer (amongst other things). This last is to support sub-string operations where a given string won't know the start of the string it was originally allocated with.


## How `u8string` and `u8view` work


What `u8string` does is to break this fat pointer apart and make the control block pointer a separate member:

```cpp
struct string {
    char const *data;
    std::size_t count;
    control *owner;
};
```

By doing this we can also add the same thing to the string view class:

```cpp
struct string_view {
    char const *data;
    std::size_t count;
    control *owner;
};
```

We still have a string view that is cheap to copy, but we can now turn a string view back into a string with zero allocations. This means that the `u8view` can always be used as the type for parameters (assuming a `u8string` is further up the call stack keeping the memory alive) without regard for whether any part of the call stack needs to take a copy or not.

Copying a `u8string` must increment the reference control pointed to through the `owner` member and its destructor must decrement the counter (and possibly release the memory).


### A final trick

Now that we have an explicit control block under out control we can also assign semantic meaning to a `nullptr` for the `owner` member. Because having a `nullptr` in the `owner` field doesn't give access to a control block, this value is perfect for designating that the string data is from a literal and the memory needs no management.

Functions can now return a `f5::u8string` from a literal without incurring any memory allocation or data copy overhead.


## Transition

Given that we have a load of code that still uses `std::string` as internal storage (tagged strings as well as `fostlib::string`) we need to be able to create `u8view` instances from these in a safe manner. This means that the control block being `nullptr` on a `u8view` can happen for one of two reasons:

1. The view was created from a literal type, or from a `u8string` that was created from a literal type.
2. The view was created from `std::string` storage that cannot be moved into a `u8sting` yet.

Because of this, we need to handle the `nullptr` control block value slightly differently: If we get a `nullptr` in a `u8string` instance that could have come from a `u8view` we must allocate new storage for the string. There are two avenues that this can happen through:

1. Construction of a `u8string` from a `u8view`.
2. Construction via iterators because the `u8string` and the `u8view` share the same iterator type.

This will make the use of `u8string` slower than it would otherwise be, but the performance cost of the extra allocations (that might not actually be needed) should still be no slower than the current practice of using a `std::string` for storage which must allocate in these situations as well (as well as many others).


## `shrink_to_fit` and C strings

There are circumstances under which we may hold on to a `u8string` that has come from a large allocation and we don't want to keep the entire memory block alive for a small part of it.

Under these circumstances we can use the `shrink_to_fit` API on `u8string` to force a re-allocation of the memory so that we get a memory block large enough for just the part that we're interested in.

We have a similar situation when we need a C compatible string (NUL terminated string). `u8string`s are not NUL terminated, but because `shrink_to_fit` is going to allocate anyway we will place an extra NUL at the end of the new memory we allocate, and `shrink_to_fit` will return the C compatible string.

```cpp
void foo(const char *c_str);

int main() {
    f5::u8string s{"hello world"};
    f5::u8string sub{s.substr(0, 5)};
    // Call foo passing it a C string
    foo(s.shrink_to_fit());
}
```
