#pragma once

#if defined(arch_x86_64)
    #include <rails/arch/x86_64/machine_context.hpp>
#elif defined(arch_armv8_a_64)
    #include <rails/arch/armv8_a_64/machine_context.hpp>
#endif
