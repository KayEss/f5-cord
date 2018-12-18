# Library design


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


## Transition

Given that we have a load of code that still uses `std::string` as internal storage (tagged strings as well as `fostlib::string`) we need to be able to create `u8view` instances from these in a safe manner. This means that the control block being `nullptr` on a `u8view` can happen for one of two reasons:

1. The view was created from a literal type, or from a `u8string` that was created from a literal type.
2. The view was created from `std::string` storage that cannot be moved into a `u8sting` yet.

Because of this, we need to handle the nullptr control block value slightly differently: If we get a `nullptr` in a `u8string` instance that could have come from a `u8view` we must allocate new storage for the string. There are two avenues that this can happen through:

1. Construction of a `u8string` from a `u8view`.
2. Construction via iterators because the `u8string` and the `u8view` share the same iterator type.

This will make the use of `u8string` slower than it would otherwise be, but the performance cost of the extra allocations (that might not actually be needed) should still be no slower than the current practice of using a `std::string` for storage which must allocate in these situations as well.
