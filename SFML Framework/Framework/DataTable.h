#pragma once
class DataTable
{
public:
	enum class Types
	{
		String,
		Zombie,
		Item,
	};
	static const int TotalType = 3;

protected:
	Types type;

public:

	DataTable(Types type) :type(type) {}
	~DataTable() = default;

	virtual bool Load() = 0;
	virtual void Release() = 0;
};