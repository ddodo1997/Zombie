#pragma once

class StringTable : public DataTable
{
public:
	static std::wstring Undefined;

protected:
	// <T1, T2>
	//	T1: ��Ʈ�� ���̵�
	//	T2: �� ��Ʈ�� ([0] �ѱ��� [1] ���� ...)
	std::unordered_map<std::string, std::wstring> table;

	std::string filePath = "tables/string_table_kor.csv";

public:
	StringTable() : DataTable(DataTable::Types::String) { };
	~StringTable() = default;

	bool Load() override;
	void Release() override;
	void SetPath();

	const std::wstring& Get(const std::string& id);
};