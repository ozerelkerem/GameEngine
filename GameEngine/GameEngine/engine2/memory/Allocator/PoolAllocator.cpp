#include "PoolAllocator.h"



PoolAllocator::PoolAllocator(size_t memSize, const void* mem, size_t objectSize, uint8_t objectAlignment) :
	IAllocator(memSize, mem), OBJECT_SIZE(objectSize), OBJECT_ALIGMENT(objectAlignment)
{
	this->clear();
}


PoolAllocator::~PoolAllocator()
{
	this->freelist = nullptr;
}

void * PoolAllocator::allocate(size_t size, uint8_t alignment)
{
	assert(size > 0 && "allocate called with memSize = 0.");
	assert(size == this->OBJECT_SIZE && alignment == this->OBJECT_ALIGMENT);

	if (this->freelist == nullptr)
		return nullptr;

	void* p = this->freelist;

	this->freelist = (void**)(*this->freelist);

	this->memoryUsed += this->OBJECT_SIZE;
	this->memoryAllocations++;

	return p;
}

void PoolAllocator::free(void * p)
{
	*((void**)p) = this->freelist;

	this->freelist = (void**)p;

	this->memoryUsed -= this->OBJECT_SIZE;
	this->memoryAllocations--;
}

void PoolAllocator::clear()
{
	uint8_t adjustment = GetAdjustment(this->memoryFirstAdress, this->OBJECT_ALIGMENT);

	size_t numObjects = (size_t)floor((this->memorySize - adjustment) / this->OBJECT_SIZE);

	union
	{
		void* asVoidPtr;
		uintptr_t asUptr;
	};

	asVoidPtr = (void*)this->memoryFirstAdress;

	// align start address
	asUptr += adjustment;

	this->freelist = (void**)asVoidPtr;

	void** p = this->freelist;

	for (int i = 0; i < (numObjects - 1); ++i)
	{
		*p = (void*)((uintptr_t)p + this->OBJECT_SIZE);

		p = (void**)*p;
	}

	*p = nullptr;
}
