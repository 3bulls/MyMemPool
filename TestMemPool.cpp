#include "MyMemPool.hpp"
#include "MyBlock.hpp"
#include <iostream>

class TestInt{
public:
    TestInt(int v):value(v){}
    void print() {
        std::cout << "Value: " << value << std::endl;
    }
private:
    int value;
};

int main() {
    MyMemPool& pool = MyMemPool::getInstance();
    void* ptr1 = pool.allocate(100);//128
    void* ptr2 = pool.allocate(5000);//5000
    void* ptr3 = pool.allocate(300);//512
    void* ptr4 = pool.allocate(700);//1024
    //128 + 5000 + 512 + 1024 = 6664
    // 1024*1024 + 5000 = 1053576
    std::cout << "Allocated ptr1: " << ptr1 << std::endl;
    std::cout << "Allocated ptr2: " << ptr2 << std::endl;
    std::cout << "Allocated ptr3: " << ptr3 << std::endl;
    std::cout << "Allocated ptr4: " << ptr4 << std::endl;
    std::cout << "Allocated size after allocations: " << pool.getAllocatedSize() << " bytes" << std::endl;
    std::cout << "Free size after allocations: " << pool.getFreeSize() << " bytes" << std::endl;
    pool.deallocate(ptr1);
    std::cout << "Allocated size after deallocating ptr1: " << pool.getAllocatedSize() << " bytes" << std::endl;
    std::cout << "Free size after deallocating ptr1: " << pool.getFreeSize() << " bytes" << std::endl;
    pool.deallocate(ptr2);
    std::cout << "Allocated size after deallocating ptr2: " << pool.getAllocatedSize() << " bytes" << std::endl;
    std::cout << "Free size after deallocating ptr2: " << pool.getFreeSize() << " bytes" << std::endl;
    pool.deallocate(ptr3);
    std::cout << "Allocated size after deallocating ptr3: " << pool.getAllocatedSize() << " bytes" << std::endl;
    std::cout << "Free size after deallocating ptr3: " << pool.getFreeSize() << " bytes" << std::endl;
    
    
    std::cout << "Allocated size after deallocations: " << pool.getAllocatedSize() << " bytes" << std::endl;
    std::cout << "Free size after deallocations: " << pool.getFreeSize() << " bytes" << std::endl;
    
    memset(ptr4, 0, 700); // 使用分配的内存
    int* intPtr = static_cast<int*>(ptr4);
    for (int i = 0; i < 10; ++i) { //
        intPtr[i] = i;
    }
    std::cout << "First 10 integers in ptr4: ";
    for (int i = 0; i < 10; ++i) {
        std::cout << intPtr[i] << " "; 
    }
    std::cout << std::endl;
    TestInt* testPtr = new (static_cast<char*>(ptr4)+10*sizeof(int)) TestInt(42);
    testPtr->print();
    testPtr->~TestInt();
    pool.deallocate(ptr4);
}