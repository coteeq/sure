#pragma once

#if defined(SURE_ARCH_x86_64)
    #include <sure/arch/x86_64/machine_context.hpp>
#elif defined(SURE_ARCH_armv8_a_64)
    #include <sure/arch/armv8_a_64/machine_context.hpp>
#elif defined(SURE_ARCH_ucontext)
    #include <sure/arch/ucontext/machine_context.hpp>
#else
    #include <sure/arch/no/machine_context.hpp>
#endif
