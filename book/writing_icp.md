\page write_icp_instance Writing an ICP Instance

To write an ICP instance, create a C++ source file with at least the following:

1. A `final` class that inherits from `public icp::ICP`
2. A static initialization variable (described below)

\section core_func_sec "Core Functionality"

The class must define the following behavior:

- A constructor that calls the `icp::ICP` constructor
- An overridden destructor (which may do nothing)
- `void iterate() override`

In `iterate`, the point clouds are given by the instance variables `a` and `b`.
There is also the `match` instance variable, allocated to have size `a.size()`, which cannot be assumed to contain any definite values.
At the end of `iterate`, the `transform` instance variable should have been updated (although the update may be zero).

Optionally, the class can override:

- `void setup() override`

`setup` is invoked upon the user call to `ICP::begin` after the internals of ICP have been readied.

\section static_init_sec "Static Initialization"

The static initialization is required so that users can instantiate your ICP instance.
Define

```cpp
static bool static_initialization = []() {
    assert(ICP::register_method("name_of_instance", []() -> std::unique_ptr<ICP> {
        return std::make_unique<NameOfClass>();
    }));
    return true;
}();
```

where `"name_of_instance"` is the name of your ICP implementation and `NameOfClass` is the name of the class.
