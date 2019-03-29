#include "IAllocator.h"



IAllocator::IAllocator(const size_t memSize, const void* mem) : memorySize(memSize), memoryFirstAdress(mem), memoryUsed(0), memoryAllocations(0)
{
}


IAllocator::~IAllocator()
{
}
