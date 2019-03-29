#pragma once

#include <memory/Allocator/IAllocator.h>

class LinearAllocator : public IAllocator
{
public:
	LinearAllocator(const size_t memSize, const void* mem);
	virtual ~LinearAllocator();

	virtual void *allocate(size_t size, uint8_t aligment) override;
	virtual void free(void *p) override;
	virtual void clear() override;
};

