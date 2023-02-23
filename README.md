# Sure 

(_Su_ spend + _Re_ sume)

`ExecutionContext` for stackful coroutines and fibers

- [Guide (ru)](docs/ru/guide.md)
- [Examples](https://gitlab.com/Lipovsky/sure-flow)

## Features

- Exceptions support
- Annotations for [AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html)
- Annotations for [ThreadSanitizer](https://clang.llvm.org/docs/ThreadSanitizer.html)

## Requirements

- x86-64, ARMv8-A-64
- Clang++

## References

### Arch

- [System V ABI / AMD64](https://www.uclibc.org/docs/psABI-x86_64.pdf)
- [ARMv8-A ISA](https://documentation-service.arm.com/static/613a2c38674a052ae36ca307)

### Exceptions

- [C++ exceptions under the hood](https://monkeywritescode.blogspot.com/p/c-exceptions-under-hood.html)
- [Exception Handling in LLVM](https://llvm.org/docs/ExceptionHandling.html)
