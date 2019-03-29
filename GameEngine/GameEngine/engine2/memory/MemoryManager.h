#pragma once

#include<memory/Allocator/StackAllocator.h>

class MemoryManager
{
	friend class GlobalMemoryUser;

public:

	static constexpr size_t MEMORY_CAPACITY = GLOBAL_MEMORY_CAPACITY;

private:

	void* globalmemoryFirstAddress;
	StackAllocator* memoryAllocator;


	MemoryManager(const MemoryManager&) = delete;
	MemoryManager& operator=(MemoryManager&) = delete;


public:
	MemoryManager();
	~MemoryManager();

	inline void* Allocate(size_t memSize)
	{
		return memoryAllocator->allocate(memSize, alignof(uint8_t));
	}

	inline void Free(void *p)
	{
		//TODO WORK WITH STACKALLOCATOR
	}

};

