#include <iostream>

int main()
{
    // Display the greeting with the current date and time
    std::cout << "Hello, World!\n";
    std::cout << "This is the test for the CPU feature flag macros!\n";

#ifdef HAS_SSE2
    std::cout << "HAS_SSE2: This CPU supports SSE2 features.\n";
#endif

#ifdef HAS_SSE3
    std::cout << "HAS_SSE3: This CPU supports SSE3 features.\n";
#endif

    return 0;
}