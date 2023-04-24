#include <CL/sycl.hpp>
#include <iostream>

int main() {
  // Create a SYCL queue
  sycl::queue myQueue;

  // Create a buffer for the message
  char message[] = "Hello, world!";
  size_t message_size = sizeof(message) / sizeof(char);
  sycl::buffer<char, 1> message_buffer(message, sycl::range<1>(message_size));

  // Define a command group to execute
  myQueue.submit([&](sycl::handler& myHandler) {
    // Get an accessor to the buffer
    auto message_accessor = message_buffer.get_access<sycl::access::mode::read>(myHandler);

    myHandler.single_task([=]() {
      // Print "Hello, world!" on the device
      for (size_t i = 0; i < message_size; ++i) {
        sycl::ext::oneapi::experimental::printf("%c", message_accessor[i]);
      }
    });
  });

  // Wait for the queue to finish executing
  myQueue.wait();

  return 0;
}
