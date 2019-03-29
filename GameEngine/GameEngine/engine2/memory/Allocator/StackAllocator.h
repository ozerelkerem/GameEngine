#pragma once
#include <memory/Allocator/IAllocator.h>
class StackAllocator : public IAllocator
{
private:
	struct AllocMetaInfo
	{
		uint8_t adjusment;
	};

public:
	StackAllocator(const size_t memSize, const void* mem);
	virtual ~StackAllocator();


	virtual void *allocate(size_t size, uint8_t aligment) override;
	virtual void free(void *p) override;
	virtual void clear() override;
};

