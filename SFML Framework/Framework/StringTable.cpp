#include "stdafx.h"
#include "StringTable.h"

std::wstring StringTable::Undefined = L"Undefined Id";

bool StringTable::Load()
{
	SetPath();
	Release();
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	rapidcsv::Document doc(filePath, rapidcsv::LabelParams(0, -1));
	for (int i = 0; i < doc.GetRowCount(); ++i)
	{
		std::vector<std::string> strings = doc.GetRow<std::string>(i);
		auto it = table.find(strings[0]);
		if (it != table.end())
		{
			std::cout << "스트링 테이블 키 중복!" << std::endl;
			return false;
		}
		table.insert({ strings[0] , converter.from_bytes(strings[1]) });
	}

	return true;
}

void StringTable::Release()
{
	table.clear();
}

void StringTable::SetPath()
{
	switch (Variables::currentLang)
	{
	case Languages::Korean:
		filePath = "tables/string_table_kor.csv";
		break;
	case Languages::English:
		filePath = "tables/string_table_eng.csv";
		break;
	case Languages::Japanese:
		filePath = "tables/string_table_jpn.csv";
		break;
	}
}

const std::wstring& StringTable::Get(const std::string& id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return Undefined;
	}
	return find->second;
}