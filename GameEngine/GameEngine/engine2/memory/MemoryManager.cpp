#include "MemoryManager.h"



MemoryManager::MemoryManager()
{
	this->globalmemoryFirstAddress = malloc(MemoryManager::MEMORY_CAPACITY);
	if (!this->globalmemoryFirstAddress)
		assert(false && "Failed to allocate memory");

	this->memoryAllocator = new StackAllocator(MemoryManager::MEMORY_CAPACITY, this->globalmemoryFirstAddress);
}


MemoryManager::~MemoryManager()
{
	this->memoryAllocator->clear();

	delete this->memoryAllocator;
	this->memoryAllocator = nullptr;

	free(this->globalmemoryFirstAddress);
	this->globalmemoryFirstAddress = nullptr;
}
