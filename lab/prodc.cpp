#include <CL/sycl.hpp>
#include <vector>
#include <iostream>

using namespace cl::sycl;

const int BUFFER_SIZE = 10;
const int NUM_ITERATIONS = 5;

// Producer function
void producer(queue& q, buffer<int, 1>& buffer, int iteration) {
    std::vector<int> data(BUFFER_SIZE);
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        data[i] = i + (BUFFER_SIZE * iteration);
    }

    // Write data to buffer
    auto write_event = q.submit([&](handler& cgh) {
        auto accessor = buffer.get_access<access::mode::write>(cgh);
        cgh.copy(data.data(), accessor);
    });

    // Wait for write event to complete
    write_event.wait();

    // Print the produced data
    std::cout << "Iteration " << iteration << " - Produced data:\n";
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

// Consumer function
void consumer(queue& q, buffer<int, 1>& buffer, int iteration) {
    std::vector<int> data(BUFFER_SIZE);

    // Read data from buffer
    auto read_event = q.submit([&](handler& cgh) {
        auto accessor = buffer.get_access<access::mode::read>(cgh);
        cgh.copy(accessor, data.data());
    });

    // Wait for read event to complete
    read_event.wait();

    // Print the consumed data
    std::cout << "Iteration " << iteration << " - Consumed data:\n";
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    queue q{sycl::gpu_selector_v};

    // Create buffer
    buffer<int, 1> buffer{range<1>{BUFFER_SIZE}};

    // Launch producer and consumer functions for multiple iterations
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        producer(q, buffer, i);
        consumer(q, buffer, i);
        std::cout << std::endl;
    }

    return 0;
}
