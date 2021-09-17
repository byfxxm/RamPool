#pragma once
#pragma warning(disable:4200)
#include "LinkedList.h"

enum class valid_t
{
	SLOT_UNUSE = 0,
	SLOT_USED = 0x7BABABAB,
	SLOT_DELETED = 0x7EDEDEDE,
};

struct Slot : public CLinkedList<Slot>::Node
{
	Slot() : m_pOwner(nullptr), m_nValid(valid_t::SLOT_UNUSE), m_nSize(0), m_nActualSize(0) {}

	void* m_pOwner;
	valid_t m_nValid;
	size_t m_nSize;
	size_t m_nActualSize;
	char m_Mem[0];
};