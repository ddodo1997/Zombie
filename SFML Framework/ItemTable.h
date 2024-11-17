#pragma once
#include "DataTable.h"
#include "Item.h"
struct ItemData
{
	std::string texId;
	int heal;
	int ammo;
};
class ItemTable :
    public DataTable
{
protected:
	std::unordered_map<Item::Types, ItemData> table;
	std::string filePath = "tables/item_table.csv";
	static ItemData UnDefine;
public:
	ItemTable() :DataTable(DataTable::Types::Item) {}
	~ItemTable() = default;

	bool Load();
	void Release();

	const ItemData& Get(Item::Types type);
};

