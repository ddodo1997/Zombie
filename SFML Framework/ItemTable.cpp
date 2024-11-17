#include "stdafx.h"
#include "ItemTable.h"
ItemData ItemTable::UnDefine;
bool ItemTable::Load()
{
    Release();

    rapidcsv::Document doc(filePath, rapidcsv::LabelParams(0, -1));
    for (int i = 0; i < doc.GetRowCount(); i++)
    {
        auto row = doc.GetRow<std::string>(i);
        Item::Types id = (Item::Types)std::stoi(row[0]);

        if (table.find(id) != table.end())
        {
            return false;
        }
        table[id].texId = row[1];
        table[id].ammo = std::stoi(row[2]);
        table[id].heal = std::stoi(row[3]);
    }

    return true;
}

void ItemTable::Release()
{
    table.clear();
}

const ItemData& ItemTable::Get(Item::Types type)
{
    auto find = table.find(type);
    if (find == table.end())
        return UnDefine;
    return find->second;
}
