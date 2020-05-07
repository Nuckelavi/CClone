#pragma once

class Block
{
private:
	bool _isSolid;

public:
	Block();
	~Block();

	void SetSolid(bool isSolid);
	bool IsSolid() const { return _isSolid; }
};