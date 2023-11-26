#include <iostream>
#include <chrono>

// The function to measure
void myFunction() {
    // Your function's code here
    for (int i = 0; i < 1332222345; ++i) {
        // Some computational work
    }
}

int main() {
    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    // Call the function you want to measure
    myFunction();

    // Stop measuring time
    auto stop = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Time taken by function: " << duration.count() << " microseconds." << std::endl;

    return 0;
}
