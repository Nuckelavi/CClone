#include "d_Block.h"

Block::Block() :
	_isSolid(true)
{
}

Block::~Block()
{
}

void Block::SetSolid(bool isSolid)
{
	_isSolid = isSolid;
}
