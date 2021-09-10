#pragma once
#pragma warning(disable:4200)
#include "Node.h"

enum class valid_t
{
	SLOT_UNUSE = 0,
	SLOT_USED = 0x7BABABAB,
	SLOT_DELETED = 0x7EDEDEDE,
};

class CSlot : public CNode
{
public:
	CSlot() : m_nValid(valid_t::SLOT_UNUSE), m_nSize(0), m_nActualSize(0) {}
	virtual ~CSlot() = default;

	valid_t m_nValid;
	size_t m_nSize;
	size_t m_nActualSize;

	char m_Mem[0];
};