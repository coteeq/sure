#pragma once

#if defined(x86_64)
    #include <context/arch/x86_64/machine_context.hpp>
#endif // x86_64

#if defined(armv8_a_64)
    #include <context/arch/armv8_a_64/machine_context.hpp>
#endif // armv8_a_64
