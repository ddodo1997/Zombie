#pragma once
#include "Scene.h"
class Player;
class Zombie;
class TileMap;
class Bullet;
class ItemMaker;
class UiHud;
class UiUpgrade;
class UiGameOver;
class SceneGame :
    public Scene
{
protected:
	Player* player;
	TileMap* tileMap;
	UiHud* uiHud;
	UiUpgrade* uiUpgrade;
	UiGameOver* uiGameover;

	std::list<Zombie*> activeZombies;
	ObjectPool<Zombie> zombiePool;
	std::list<Bullet*> activeBullet;
	ObjectPool<Bullet> bulletPool;

	ItemMaker* itemMaker;

	sf::Sprite cursor;
public:
	SceneGame();
	virtual ~SceneGame() = default;

	void Init();
	void Release();

	void Enter();
	void Exit();

	void Update(float dt);

	void Draw(sf::RenderWindow& window);

	void SpawnZomies(int count);

	Bullet* TakeBullet();
	void ReturnBullet(Bullet* bullet);
	TileMap* GetMap() { return tileMap; }
	ItemMaker* GetMaker() { return itemMaker; }
	const std::list<Zombie*>& GetZombieList() const { return activeZombies; }

	void OnZombieDie(Zombie* zombie);
	void OnPlayerDie();
	void OnUpgrade(sf::Text& text, int upgrade);
};

