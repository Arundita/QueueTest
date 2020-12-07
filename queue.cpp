#include<iostream>
#include "pch.h"

typedef short Q;
typedef short bytesUsedType;
typedef int shiftCastType;

const short unusedQueue = -1;
const unsigned short totalMemory = 2048;
const unsigned short maxQueues = 64;


const unsigned short bytesUsedIndex = sizeof(Q) * maxQueues;
const unsigned short dataShiftSize = sizeof(shiftCastType);
const unsigned short padding = dataShiftSize - 1;
const unsigned short dataStartOffset = bytesUsedIndex + sizeof(bytesUsedType);


unsigned char memo[totalMemory];
bool outOfMemo = false;
bool illegalOp = false;
std::string illegalOperation = "";
std::string outOfMemory = "";
unsigned char retChar;

Q* create_queue();                        //Creates a FIFO byte queue, returning a handle to it.
void destroy_queue(Q* q);                 //Destroy an earlier created byte queue.
void enqueue_byte(Q* q, unsigned char b); //Adds a new byte to a queue.
unsigned char dequeue_byte(Q* q);         //Pops the next byte off the FIFO queue.
Q offset;
bytesUsedType* bytesUsed;


void on_illegal_operation() {
    illegalOperation = "Queue is empty. Illegal Opeartion !!!";
    std::cout << illegalOperation << "\n";
    illegalOp = true;
}

void on_out_of_memory() {
    outOfMemory = "Not enough memory !!!";
    std::cout << outOfMemory << "\n";
    outOfMemo = true;
}

/*int main(){
    std::cout << " Printing starts ----- "<<"\n";
    Q* q0 = create_queue();
    enqueue_byte(q0, '0');
    enqueue_byte(q0, '1');
    Q* q1 = create_queue();
    enqueue_byte(q1, '3');
    enqueue_byte(q0, '2');
    enqueue_byte(q1, '4');
    std::cout<<dequeue_byte(q0)<<" ";
    std::cout << dequeue_byte(q0) <<"\n";
    enqueue_byte(q0, '5');
    enqueue_byte(q1, '6');
    std::cout << dequeue_byte(q0) << " ";
    std::cout << dequeue_byte(q0) << "\n";
    destroy_queue(q0);
    std::cout << dequeue_byte(q1) << " ";
    std::cout << dequeue_byte(q1) << " ";
    std::cout << dequeue_byte(q1) << "\n";
    destroy_queue(q1);
    return 0;
}*/


Q* create_queue() {

    static bool dataArrayIni = false;

    if (!dataArrayIni) {

        for (unsigned int i = 0; i < maxQueues; ++i) {
            *reinterpret_cast<Q*>(&memo[sizeof(Q) * i]) = unusedQueue;
        }
        *reinterpret_cast<bytesUsedType*>(memo + bytesUsedIndex) = dataStartOffset;
        dataArrayIni = true;
    }

    //bytesUsed represents the total amount of bytes in use by the array.
    bytesUsed = reinterpret_cast<bytesUsedType*>(memo + bytesUsedIndex);

    if (*bytesUsed >= totalMemory - padding - 2) on_out_of_memory();

    //Qsize contains the size of the Queue it applies to
    Q* Qsize = reinterpret_cast<Q*>(memo);
    Q offset = dataStartOffset;

    for (unsigned short i = 0; i < maxQueues; ++i) {

        if (*Qsize == unusedQueue)

        {
            float numCopies = float(*bytesUsed - offset) / float(dataShiftSize);
            shiftCastType* src = reinterpret_cast<shiftCastType*>(memo + *bytesUsed - dataShiftSize);
            shiftCastType* dst = reinterpret_cast<shiftCastType*>(memo + *bytesUsed - 1);

            for (float i = 0.0f; i < numCopies; i++) {
                *(dst--) = *(src--);
            }


            *Qsize = padding;
            *bytesUsed += padding;
            return Qsize;
        }

        else offset += *(Qsize++);

    }

    on_illegal_operation();
    return 0;
}

void destroy_queue(Q* q) {

    bytesUsedType* bytesUsed = reinterpret_cast<bytesUsedType*>(memo + bytesUsedIndex);
    Q* Qsize = reinterpret_cast<Q*>(memo);
    Q offset = dataStartOffset;

    if (!q) on_illegal_operation();
    if (*q == unusedQueue) on_illegal_operation();

    while (Qsize != q) {
        if (*Qsize > 0) offset += *Qsize;
        Qsize++;
    }


    float numCopies = float(*bytesUsed - offset - *q) / float(dataShiftSize);

    shiftCastType* src = reinterpret_cast<shiftCastType*>(memo + offset + *q);
    shiftCastType* dst = reinterpret_cast<shiftCastType*>(memo + offset);


    for (float i = 0.0f; i < numCopies; i++) {
        *(dst++) = *(src++);
    }

    *bytesUsed -= *q;
    *q = unusedQueue;
}

void enqueue_byte(Q* q, unsigned char b) {

    bytesUsedType* bytesUsed = reinterpret_cast<bytesUsedType*>(memo + bytesUsedIndex);
    Q* Qsize = reinterpret_cast<Q*>(memo);
    offset = dataStartOffset;

    if (!q)    on_illegal_operation();
    if (*bytesUsed >= totalMemory) on_out_of_memory();

    while (Qsize != q) {
        if (*Qsize > 0) offset += *Qsize;
        Qsize++;
    }

    offset += *Qsize;
    float numCopies = float(*bytesUsed - offset) / float(dataShiftSize);

    shiftCastType* src = reinterpret_cast<shiftCastType*>(memo + *bytesUsed - dataShiftSize);
    shiftCastType* dst = reinterpret_cast<shiftCastType*>(memo + *bytesUsed - (dataShiftSize - 1));


    for (float i = 0.0f; i < numCopies; i++) {
        *(dst--) = *(src--);
    }

    memo[offset - padding] = b;
    *bytesUsed += 1;
    *q += 1;
}


unsigned char dequeue_byte(Q* q) {

    bytesUsedType* bytesUsed = reinterpret_cast<bytesUsedType*>(memo + bytesUsedIndex);
    Q* Qsize = reinterpret_cast<Q*>(memo);
    offset = dataStartOffset;

    if (!q) on_illegal_operation();
    if (*q <= padding) on_illegal_operation();


    while (Qsize != q) {
        if (*Qsize > 0) offset += *Qsize;
        Qsize++;
    }

    retChar = memo[offset];
    float numCopies = float(*bytesUsed - offset - 1) / float(dataShiftSize);

    shiftCastType* src = reinterpret_cast<shiftCastType*>(memo + offset + 1);
    shiftCastType* dst = reinterpret_cast<shiftCastType*>(memo + offset);


    for (float i = 0.0f; i < numCopies; i++) {
        *(dst++) = *(src++);
    }

    *bytesUsed -= 1;
    *q -= 1;
    return retChar;
}