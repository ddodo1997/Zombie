#pragma once
namespace sf {
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(sf::Vector2f, x, y)
}

struct SaveZombie
{
	int type = 0;
	sf::Vector2f position;
	float rotation;
	sf::Vector2f scale;
	int hp = 0;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SaveZombie, type, position, rotation, scale, hp);
};

class SaveData
{
public:
	int version = 0;
	virtual SaveData* VersionUp() = 0;
};

class SaveDataV1 : public SaveData
{
public:
	SaveDataV1() { version = 1; }

	std::vector<SaveZombie> zombies;

	SaveData* VersionUp() override { return nullptr; }
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SaveDataV1, version, zombies);
};


typedef SaveDataV1 SaveDataVC;