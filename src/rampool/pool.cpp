#include "stdafx.h"
#include "pool.h"
#include "block.h"
#include "slot.h"

void pool_c::initialize(size_t size, const void* owner)
{
	__size = size;
	__owner = owner;
}

size_t pool_c::get_size()
{
	return __size;
}

void* pool_c::malloc(size_t size)
{
	lock_t lock(__mutex);
	++__count;
	__total += size;

	auto slot_s_ = __free_stack.pop();
	if (slot_s_)
	{
		assert(slot_s_->valid == valid_t::DELETED);
		slot_s_->valid = slot_s::valid_t::USED;
		slot_s_->actual_size = size;
		return slot_s_->mem;
	}

	auto block_ = __block_stack.top();
	if (!block_ || block_->is_full())
	{
		block_ = new block_s(__size, __owner);
		__block_stack.push(block_);
	}

	return block_->alloc(size);
}

void pool_c::free(void* p)
{
	lock_t lock(__mutex);

	auto slot_s_ = POINTER_TO_slot_s(p);
	slot_s_->valid = slot_s::valid_t::DELETED;
	__free_stack.push(slot_s_);
	--__count;
	__total -= slot_s_->actual_size;
	assert((int)__count >= 0);
}

void pool_c::destroy()
{
	lock_t lock(__mutex);

	block_s* block_ = nullptr;
	while (block_ = __block_stack.top())
	{
		__block_stack.pop();
		delete block_;
	}

	new(&__block_stack) stack_c<block_s>();
	new(&__free_stack) stack_c<slot_s>();
	__count = 0;
	__total = 0;
}

size_t pool_c::count()
{
	return __count;
}

size_t pool_c::total()
{
	return __total;
}

void pool_c::gc()
{
	lock_t lock(__mutex);

	block_s* next = nullptr;
	for (auto block = __block_stack.top(); block; block = next)
	{
		next = block->next;

		size_t idx = 0;
		for (; idx < block->cur_slot; ++idx)
		{
			assert(block_->slots[idx]->valid != valid_t::UNUSE);
			if (block->slots[idx]->valid == slot_s::valid_t::USED)
				break;
		}

		if (idx == block->cur_slot)
		{
			for (size_t i = 0; i < block->cur_slot; ++i)
				block->slots[i]->valid = slot_s::valid_t::UNUSE;

			for (auto slot = __free_stack.top(); slot; slot = slot->next)
			{
				if (slot->valid == slot_s::valid_t::UNUSE)
					__free_stack.erase(slot);
			}

			__block_stack.erase(block);
			delete block;
		}
	}
}

bool pool_c::need_gc()
{
	return __free_stack.count() >= AUTOGC_THRESHOLD;
}