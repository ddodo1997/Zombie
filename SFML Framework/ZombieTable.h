#pragma once
#include "DataTable.h"
#include "Zombie.h"
struct DataZombie
{
	Zombie::Types id;

	std::string texZombieId;

	int maxHp = 0;
	int attack = 0;
	float attackDelay = 0.f;
	float speed = 0.f;
};
class ZombieTable :
    public DataTable
{
public:
	static DataZombie UnDefined;
protected:
	std::unordered_map<Zombie::Types, DataZombie> table;
	std::string filePath = "tables/zombie_table.csv";
public:

	ZombieTable():DataTable(DataTable::Types::Zombie) {}
	~ZombieTable() = default;

	bool Load();
	void Release();

	const DataZombie& Get(Zombie::Types id);
};

