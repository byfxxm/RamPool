﻿#include "stdafx.h"
#include "RamPool.h"
#include "RamPoolImp.h"

void* RamPool_Create()
{
	return new CRamPoolImp();
}

void RamPool_Delete(void* pRamPool_)
{
	delete (CRamPoolImp*)pRamPool_;
}

void* RamPool_Malloc(void* pRamPool_, size_t nSize_)
{
	return ((CRamPoolImp*)pRamPool_)->Malloc(nSize_);
}

void RamPool_Free(void* pRamPool_, void* p_)
{
	return ((CRamPoolImp*)pRamPool_)->Free(p_);
}

void* RamPool_Realloc(void* pRamPool_, void* p_, size_t nSize_)
{
	return ((CRamPoolImp*)pRamPool_)->Realloc(p_, nSize_);
}

void RamPool_Destroy(void* pRamPool_)
{
	return ((CRamPoolImp*)pRamPool_)->Destroy();
}

size_t RamPool_Leak(void* pRamPool_)
{
	return ((CRamPoolImp*)pRamPool_)->Leak();
}

void* rp_malloc(size_t nSize_)
{
	return CRamPoolImp::Instance()->Malloc(nSize_);
}

void rp_free(void* p_)
{
	return CRamPoolImp::Instance()->Free(p_);
}

void* rp_realloc(void* p_, size_t nSize_)
{
	return CRamPoolImp::Instance()->Realloc(p_, nSize_);
}

void rp_destroy()
{
	return CRamPoolImp::Instance()->Destroy();
}

size_t rp_leak()
{
	return CRamPoolImp::Instance()->Leak();
}