Implement a FIFO Queue of unsigned char and written test cases for the same. I have implemented the following functions for the FIFO Queue of unsigned characters. 

Q* create_queue() //creates a FIFO Byte Queue returning a handle to it.
Void destroy_queue(Q* q) //Destroy a previously created queue
Void enqueue_byte(Q* q, unsigned char b) //Adds a new byte to the queue
Unsigned char dequeue_byte(Q* q) //pops the next byte off the queue.

If the functions run out of memory, it will call this function (which you may define/override):
                  void on_out_of_memory();
If the caller makes an illegal request, it will call this function (which you may define/override):
                void on_illegal_operation();

The specifications also require that the implementation
• uses no more than 2048 bytes to implement all byte queues, and
• must support 15 queues with an average of 80 or so bytes in each queue.

I have chosen the Google testing framework for testing the code for this implementation. I have chosen Google Test for the following reasons.

1. Similar test cases can be grouped together under one test that can share data and test routines. This makes the test more organized and reflects the structure of the tested code.
Test cases are independent of each other which makes debugging easy.
2. Test cases are portable and reusable, Google Test work with different operating systems and compilers (MSVC, GCC, etc) chance it can be used with a variety of configurations. 
3. Test cases can be divided as fatal (ASSERT_*) and non-fatal (EXPECT_*) failures so as to manipulate the test case execution. On failure, the google test only stops the current test and continues to the next. On non-fatal failure, the current test continues. Thus multiple failures can be reported on a single edit-compile-test cycle.
4. Google Test automatically detects your tests and doesn’t require you to enumerate them in order to run them.

I have written 22 Tests from 10 Test Cases. Following are the details of Test Cases.

1. Test Case: Before the queue is created.

1.1 BytesUsed variable represents the total bytes in use by the memory, before a new queue is created, Verify that there's room for three padding bytes and at-least one byte to enqueue.

1.2 If a free queue is found after iterating through all the available memory space, verify that the free space is allocated to the current queue.

1.3 If a free queue is not found, verify that call on_illegal_operation() is called to indicate no available free memory space.

2. Test Case: Create the Queue

Verify that the size of the new queue is equal to the number of padding bytes that are added at the end of each queue.

3. Enqueue a byte to the queue

3.1 Before the queue can be enqueued, verify that the queue is not null.<br>
3.2 If the queue is not null, verify that the queue size is less than the total available memory space, to make sure there is room to add a byte.

4. Test Case: Enqueue Byte:

4.1 Verify that the byte is enqueued to the queue as expected. This is done by verifying that after the enqueue operation the byte present at the rear of the queue is the same as the byte inserted.<br>
4.2 Verify that the bytesUsed variable that represents the bytes in use by the memory is increased by one.<br>
4.3 Verify that the queue size is increased by one.<br>
4.4 Verify the enqueue byte operation for invalid data.<br>


5. Test Case: Before a byte is dequeued from the queue

5.1 Before a byte can be dequeued from the queue, Verify that the queue is not null.<br>
5.2 Before the queue can be dequeued, Verify that the queue is not an empty queue with nothing to be dequeued.<br>

6. Test Case: Dequeue a byte from the queue, After the dequeue operation,

6.1 Verify that the bytesUsed variable is decremented by one.<br>
6.2 Verify that the queue size is decremented by one.<br>
6.3 The offset variable indicates the location in the memory where the new queue will be stored. Verify that the offset variable is incremented by four.<br>
6.4 Verify that the character returned from the dequeue operation is as expected.<br>

7. Test Case: Before the queue is destroyed,

7.1 Verify that the queue is not null.<br>
7.2 Verify that the queue to be destroyed is an allocated queue in the memory.<br>

8. Test Case: Destroy Queue: After the queue is destroyed.

8.1: Verify that the bytesUSed variable is decremented by the size of the destroyed queue.<br>
8.2 The unusedQueue variable represents that the queue has not been allocated. Verify that the queue is assigned as an unused queue.<br>

9. Test Case: Verify that the functionality of on_out_of_memory() is as expected.<br>
10. Test Case: Verify that the functionality of on_illegal_opeartion() is as expected.<br>
