#include <iostream>
#include <cstring>

int main() {
    char cpuVendor[13];     // Buffer for CPU vendor string
    char cpuBrand[49];      // Buffer for CPU brand string
    unsigned int featuresEDX;   // CPU features from EDX register
    unsigned int featuresECX;   // CPU features from ECX register

    // Retrieve CPU vendor string using CPUID instruction
    asm (
        // CPUID call to get CPU vendor string
        "mov $0, %%eax\n\t"      // Load 0 into EAX to request vendor string
        "cpuid\n\t"              // Execute CPUID instruction
        "mov %%ebx, %0\n\t"      // Move result into cpuVendor (part 1)
        "mov %%edx, %1\n\t"      // Move result into cpuVendor (part 2)
        "mov %%ecx, %2\n\t"      // Move result into cpuVendor (part 3)
        // Output operands
        : "=m" (cpuVendor[0]), "=m" (cpuVendor[4]), "=m" (cpuVendor[8])  // Output operands for vendor string
        // Input operands (none)
        :
        // Clobbers (registers that get modified by the inline assembly)
        : "%eax", "%ebx", "%ecx", "%edx"  // List of clobbered registers
    );

    cpuVendor[12] = '\0';  // Null-terminate the CPU vendor string

    // Retrieve CPU brand string using multiple CPUID calls
    asm (
        // First CPUID call to get the CPU brand string (part 1)
        "mov $0x80000002, %%eax\n\t"   // Load 0x80000002 into EAX (start of brand string)
        "cpuid\n\t"                    // Execute CPUID instruction
        "mov %%eax, %0\n\t"            // Move result into cpuBrand[0-3] (part 1)
        "mov %%ebx, %1\n\t"            // Move result into cpuBrand[4-7] (part 1)
        "mov %%ecx, %2\n\t"            // Move result into cpuBrand[8-11] (part 1)
        "mov %%edx, %3\n\t"            // Move result into cpuBrand[12-15] (part 1)
        
        // Second CPUID call to get the CPU brand string (part 2)
        "mov $0x80000003, %%eax\n\t"   // Load 0x80000003 into EAX (middle of brand string)
        "cpuid\n\t"                    // Execute CPUID instruction
        "mov %%eax, %4\n\t"            // Move result into cpuBrand[16-19] (part 2)
        "mov %%ebx, %5\n\t"            // Move result into cpuBrand[20-23] (part 2)
        "mov %%ecx, %6\n\t"            // Move result into cpuBrand[24-27] (part 2)
        "mov %%edx, %7\n\t"            // Move result into cpuBrand[28-31] (part 2)
        
        // Third CPUID call to get the CPU brand string (part 3)
        "mov $0x80000004, %%eax\n\t"   // Load 0x80000004 into EAX (end of brand string)
        "cpuid\n\t"                    // Execute CPUID instruction
        "mov %%eax, %8\n\t"            // Move result into cpuBrand[32-35] (part 3)
        "mov %%ebx, %9\n\t"            // Move result into cpuBrand[36-39] (part 3)
        "mov %%ecx, %10\n\t"           // Move result into cpuBrand[40-43] (part 3)
        "mov %%edx, %11\n\t"           // Move result into cpuBrand[44-47] (part 3)
        
        // Output operands (memory clobbered, not used as inputs or outputs)
        : "=m" (cpuBrand[0]), "=m" (cpuBrand[4]), "=m" (cpuBrand[8]), "=m" (cpuBrand[12]),  // Output operands for first CPUID call
          "=m" (cpuBrand[16]), "=m" (cpuBrand[20]), "=m" (cpuBrand[24]), "=m" (cpuBrand[28]), // Output operands for second CPUID call
          "=m" (cpuBrand[32]), "=m" (cpuBrand[36]), "=m" (cpuBrand[40]), "=m" (cpuBrand[44])  // Output operands for third CPUID call
        // Input operands (none)
        :
        // Clobbers (registers that get modified by the inline assembly)
        : "%eax", "%ebx", "%ecx", "%edx"  // List of clobbered registers
    );

    cpuBrand[48] = '\0';  // Null-terminate the CPU brand string

    // Retrieve CPU features from EDX and ECX using CPUID instruction
    asm (
        // CPUID call to get CPU features from EDX and ECX
        "mov $1, %%eax\n\t"    // Load 1 into EAX to request feature information
        "cpuid\n\t"            // Execute CPUID instruction
        "mov %%edx, %0\n\t"    // Move result into featuresEDX (EDX register)
        "mov %%ecx, %1\n\t"    // Move result into featuresECX (ECX register)
        // Output operands
        : "=m" (featuresEDX), "=m" (featuresECX)  // Output operands for feature bits
        // Input operands (none)
        :
        // Clobbers (registers that get modified by the inline assembly)
        : "%eax", "%ebx", "%ecx", "%edx"  // List of clobbered registers
    );

    // Print CPU information
    std::cout << "CPU Vendor: " << cpuVendor << std::endl;
    std::cout << "CPU Brand: " << cpuBrand << std::endl;
    std::cout << "CPU Features (EDX): " << std::hex << featuresEDX << std::endl;
    std::cout << "CPU Features (ECX): " << std::hex << featuresECX << std::endl;

    return 0;
}
