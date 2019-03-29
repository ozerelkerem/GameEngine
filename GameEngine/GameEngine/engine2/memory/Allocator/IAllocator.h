#pragma once
#include <Api.h>

static inline void* AlignForward(void* address, uint8_t alignment)
{
	return (void*)((reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
}

// returns the number of bytes needed to align the address
static inline uint8_t GetAdjustment(const void* address, uint8_t alignment)
{
	uint8_t adjustment = alignment - (reinterpret_cast<uintptr_t>(address)& static_cast<uintptr_t>(alignment - 1));

	return adjustment == alignment ? 0 : adjustment;
}

static inline uint8_t GetAdjustment(const void* address, uint8_t alignment, uint8_t extra)
{
	uint8_t adjustment = GetAdjustment(address, alignment);

	uint8_t neededSpace = extra;

	if (adjustment < neededSpace)
	{
		neededSpace -= adjustment;

		//Increase adjustment to fit header
		adjustment += alignment * (neededSpace / alignment);

		if (neededSpace % alignment > 0)
			adjustment += alignment;
	}

	return adjustment;
}


class IAllocator
{
protected:

	const size_t		memorySize;
	const void*			memoryFirstAdress;

	size_t				memoryUsed;
	uint32_t			memoryAllocations;

public:

	IAllocator(const size_t memSize, const void* mem);
	virtual ~IAllocator();

	virtual void* allocate(size_t size, uint8_t alignment) = 0;
	virtual void free(void* p) = 0;
	virtual void clear() = 0;

	// ACCESSOR
	inline size_t GetMemorySize() const
	{
		return this->memorySize;
	}

	inline const void* GetMemoryAddress0() const
	{
		return this->memoryFirstAdress;
	}

	inline size_t GetUsedMemory() const
	{
		return this->memoryUsed;
	}

	inline uint32_t GetAllocationCount() const
	{
		return this->memoryAllocations;
	}

};

