#include "LinearAllocator.h"



LinearAllocator::LinearAllocator(const size_t memSize, const void* mem) : IAllocator(memSize, mem)
{
}


LinearAllocator::~LinearAllocator()
{
	this->clear();
}

void * LinearAllocator::allocate(size_t size, uint8_t aligment)
{
	union
	{
		void* asVoidPtr;
		uintptr_t asUintPtr;
	};

	asVoidPtr = (void *)this->memoryFirstAdress;
	asUintPtr += memoryUsed;

	uint8_t adjusment = GetAdjustment(asVoidPtr, aligment);

	if (this->memoryUsed + size + adjusment > this->memorySize)
		return nullptr;

	asUintPtr += adjusment;

	this->memoryUsed += size + adjusment;
	this->memoryAllocations++;

	return asVoidPtr;
}

void LinearAllocator::free(void * p)
{
	assert(false && "You can only clear LinearAllocator");
}

void LinearAllocator::clear()
{
	this->memoryUsed = 0;
	this->memoryAllocations = 0;
}
