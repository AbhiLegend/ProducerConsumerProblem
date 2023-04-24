#include <CL/sycl.hpp>
#include <iostream>
#include <vector>

using namespace sycl;

constexpr size_t num_philosophers = 5;

//************************************
// Simulate eating action for philosophers in parallel.
//************************************
void philosophers_eating(queue& q, size_t num_iterations) {
    // Range object for the number of philosophers.
    range<1> num_items{ num_philosophers };

    // Create a buffer that holds the philosopher IDs.
    std::vector<size_t> ids(num_philosophers);
    for (size_t i = 0; i < num_philosophers; ++i) {
        ids[i] = i;
    }
    buffer id_buf(ids);

    for (size_t i = 0; i < num_iterations; ++i) {
        q.submit([&](handler& h) {
            // Create an accessor with read_only access permission.
            accessor<size_t, 1, access::mode::read_write, access::target::global_buffer> ids(id_buf, h);

            // Use parallel_for to simulate eating action in parallel.
            h.parallel_for(num_items, [=](auto i) {
                ids[i] = i;
            });
        });

        // Wait for the tasks to complete.
        q.wait();

        // Print the messages on the host side.
        for (const auto& id : ids) {
            std::cout << "Philosopher " << id << " is eating iteration " << i << std::endl;
        }
    }
}

//************************************
// Main function.
//************************************
int main() {
    // Create device selector for the device of your interest.
    default_selector d_selector;

    try {
        queue q(d_selector);
        // Print out the device information used for the kernel code.
        std::cout << "Running on device: "
            << q.get_device().get_info<info::device::name>() << "\n";

        size_t num_iterations = 10;
        philosophers_eating(q, num_iterations);
    }
    catch (exception const& e) {
        std::cout << "An exception is caught for philosophers_eating.\n";
        std::terminate();
    }

    std::cout << "Philosophers eating simulation successfully completed on device.\n";
    return 0;
}
