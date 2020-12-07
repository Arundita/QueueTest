#include "pch.h"
#include "iostream"
#include "queue.cpp"
#include "gtest/gtest.h"
using namespace std;

//bytesUsedType* bytesUsed = reinterpret_cast<bytesUsedType*>(memo + bytesUsedIndex);
Q* qSize = reinterpret_cast<Q*>(memo);
Q offset_prev;
bytesUsedType bytesUsed_prev;
int q_size;
unsigned char charToEnqueue = '0';
unsigned char charToReturn;

/* bytesUsed represents the total number of bytes in use by the memory, the following test checks that memory has room
for three padding bytes and atleast one byte to enqueue.*/
TEST(Q_BeforeCreateQueue, total_bytes_used) {
	ASSERT_LE(*bytesUsed, totalMemory - padding - 2);
	ASSERT_EQ(outOfMemo, false);
}

/* if a free queue is not found after iterating through all the free memory, call illegal operation*/
TEST(Q_BeforeCreateQueue, noAvailableFreeSpace_freeQueueFound) {
	for (unsigned short i = 0; i < maxQueues; i++) {
		if (*qSize != unusedQueue) {
			ASSERT_EQ(illegalOp, false);
		}
	}
}
TEST(Q_BeforeCreateQueue, noAvailableFreeSpace_freeQueueNotFound) {
	int count = 0;
	for (unsigned short i = 0; i < maxQueues; i++) {
		if (*qSize == unusedQueue) {
			count++;
		}
	}
	if (count == totalMemory) ASSERT_EQ(illegalOp, true);
	bytesUsed_prev = *bytesUsed;
}

/* now since we have space in memo to create a queue, we can create a queue. we also store the previous size of the*/
Q* q0 = create_queue();

/*size of the new queue is 3 which represents the number of padding bytes that are added at the end of each queue.*/
TEST(Q_CreateQueue, queueSize) {
	ASSERT_EQ(*qSize, padding);
}


/*Before it can be queued, q is checked to ensure it is not null.*/
TEST(Q_BeforeEnqueuByte, queueNotNull) {
	ASSERT_NE(q0, nullptr);
	ASSERT_EQ(illegalOp, false);
}
/*byte variable is checked to make sure there is room in queue to add a byte to the queue.*/
TEST(Q_BeforeEnqueuByte, queueOutOfMemo) {
	ASSERT_LE(*bytesUsed, totalMemory);
	ASSERT_EQ(outOfMemo, false);
	bytesUsed_prev = *bytesUsed;
}

Q q0_prev = *q0;

/*check the enqueued data is enqueued in the array as expected.*/
TEST(Q_EnqueuByte, valid_input) {
	offset += *qSize;
	enqueue_byte(q0, charToEnqueue);
	ASSERT_EQ(memo[offset - padding], '0');
}
/*check that the byte used and q values are both incremented by one to represent the new byte added to the queue.*/
TEST(Q_EnqueuByte, bytesUsed) {
	ASSERT_EQ(*bytesUsed, bytesUsed_prev + 1);
}
TEST(Q_EnqueuByte, queueSize) {
	ASSERT_EQ(*q0, q0_prev + 1);
}
/* False positive check for enqueued data*/
TEST(Q_EnqueuByte, invalid_input) {
	enqueue_byte(q0, '0');
	ASSERT_NE(memo[offset - padding], '1');
}
/*before a byte can be dequeued, check that queue is not null*/
TEST(Q_BeforeDequeByte, queueNotNull) {
	ASSERT_NE(q0, nullptr);
	ASSERT_EQ(illegalOp, false);
}
/* before data can be dequeued, check that  queue is not an empty queue with nothing to be dequeued*/
TEST(Q_BeforeDequeByte, queueNotEmpty) {
	ASSERT_GE(*q0, padding);
	ASSERT_EQ(illegalOp, false);
	bytesUsed_prev = *bytesUsed;
	q0_prev = *q0;
	offset_prev = offset;
}
/*Check that the bytes used and q are both decremented by one, to represent the byte the queue has been dequeued*/
TEST(Q_DequeByte, byteUsed) {
	charToReturn = dequeue_byte(q0);
	ASSERT_EQ(*bytesUsed, bytesUsed_prev - 1);
}
TEST(Q_DequeByte, queueSize) {
	ASSERT_EQ(*q0, q0_prev - 1);
}
/*After queue has been dequeued, the offset should decrement by 4.*/
TEST(Q_DequeByte, queueOffset) {
	ASSERT_EQ(offset + 4, offset_prev);
}
/* check the character has been dequeued as expected. */
TEST(Q_DequeByte, DequedChar) {
	ASSERT_EQ(charToReturn, memo[offset]);
}
/*before it can be used, q0 is checked to be not null */
TEST(Q_BeforeDestroyQueue, notNull) {
	ASSERT_NE(q0, nullptr);
	ASSERT_EQ(illegalOp, false);
}
/*queue us checked to ensure it represents an allocated queue in the memory.*/
TEST(Q_BeforeDestroyQueue, allocatedQueue) {
	ASSERT_NE(*q0, unusedQueue);
	ASSERT_EQ(illegalOp, false);
	bytesUsed_prev = *bytesUsed;
	q_size = *q0;
}
/*After queue has been destroyed, the byteUsed value is decreased by the former size of the destroyed queue,
and the queue is reset to unused queue (-1) */
TEST(Q_DestroyQueue, bytesUsed) {
	destroy_queue(q0);
	ASSERT_EQ(*bytesUsed, bytesUsed_prev - q_size);
}
TEST(Q_DestroyQueue, queueSize){
	ASSERT_EQ(*q0, unusedQueue);
}
/*check the expected output of on_out_of_memory()*/
TEST(Q_OutOfMemory, outOfMemory) {
	string outOfMemoryText = "Not enough memory !!!";
	on_out_of_memory();
	ASSERT_EQ(outOfMemoryText, outOfMemory);
	ASSERT_EQ(outOfMemo, true);
}

/*check the expected output of on_illegal_operation()*/
TEST(Q_IllegalOperation, illegalOperation) {
	string illegalOperationText = "Queue is empty. Illegal Opeartion !!!";
	on_illegal_operation();
	ASSERT_EQ(illegalOperationText, illegalOperation);
	ASSERT_EQ(illegalOp, true);
}
