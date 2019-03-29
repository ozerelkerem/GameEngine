#include "StackAllocator.h"



StackAllocator::StackAllocator(const size_t memSize, const void* mem) : IAllocator(memSize, mem)
{
}


StackAllocator::~StackAllocator()
{
	this->clear();
}

void * StackAllocator::allocate(size_t size, uint8_t aligment)
{
	union
	{
		void* asVoidPtr;
		uintptr_t asUintPtr;
		AllocMetaInfo *metainfo;
	};

	asVoidPtr = (void *)this->memoryFirstAdress;
	asUintPtr += this->memoryUsed;

	uint8_t adjusment = GetAdjustment(asVoidPtr, aligment, sizeof(AllocMetaInfo));

	if (this->memoryUsed + size + adjusment > this->memorySize)
		return nullptr;

	metainfo->adjusment = adjusment;

	asUintPtr += adjusment;

	this->memoryUsed += size + adjusment;
	this->memoryAllocations++;

	return asVoidPtr;
}

void StackAllocator::free(void * p)
{
	union
	{
		void* asVoidPtr;
		uintptr_t asUintPtr;
		AllocMetaInfo* metainfo;
	};

	asVoidPtr = p;


	asUintPtr -= sizeof(AllocMetaInfo);

	// free used memory
	this->memoryUsed -= ((uintptr_t)this->memoryFirstAdress + this->memoryUsed) - ((uintptr_t)p + metainfo->adjusment);

	// decrement allocation count
	this->memoryAllocations--;
}

void StackAllocator::clear()
{
	this->memoryUsed = 0;
	this->memoryAllocations = 0;
}
