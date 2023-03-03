# C++ Simple ServiceLocator #
A simple implementation of the service service_locator design pattern in C++.

## Usage ##

### Single Instance ###
Register an instance:
```cpp
ServiceLocator service_locator;
service_locator.registerInstance<IMathService>(new MathService());
```
And resolve it somewhere else:
```cpp
auto svc1 = service_locator.resolve<IMathService>();
auto svc2 = service_locator.resolve<IMathService>(); // <= same instance
```


### Lazy Initialization ###
Register a delegate which creates a shared pointer to the corresponding instance:
```cpp
ServiceLocator service_locator;
service_locator.registerCreator<IMathService>([]() { return std::make_shared<MathService>(); });
```
Everytime you call `resolve`, the service service_locator will call the delegate and return the pointer:
```cpp
auto svc1 = service_locator.resolve<IMathService>();
auto svc2 = service_locator.resolve<IMathService>(); // <= not the same instance
```

### Clearing all registrations

You can clear all registrations manually (will be called in destructor automatically):
```cpp
service_locator.clear();
```

## License ##

You can redistribute it and/or modify it under the terms of the MIT license. See [LICENSE][1] for details.

[1]:LICENSE