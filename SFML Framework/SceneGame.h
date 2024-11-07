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
class UiCenterMsg;
class SceneGame :
	public Scene
{
public:
	enum class Status {
		Awake,
		Wave,
		Interval,
		Upgrade,
		Pause,
		GameOver
	};
protected:
	Status currentStatus = Status::Awake;
	Status prevStatus = Status::Awake;
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

	int score = 0;
	static int highScore;
	
	int currentWave;

	float intervalTimer = 10.f;
	float IntervalDelay = 0.f;

	int upgradeCount = 0;
public:
	SceneGame();
	virtual ~SceneGame() = default;

	void Init();
	void Release();

	void Enter();
	void Exit();

	void Update(float dt);
	void UpdateAwake(float dt);
	void UpdateWave(float dt);
	void UpdateInterval(float dt);
	void UpdateUpgrade(float dt);
	void UpdatePause(float dt);
	void UpdateGameOver(float dt);
	void UpdateUI();

	void SetStatus(Status status);

	void Draw(sf::RenderWindow& window);

	void SpawnZomies(int count);

	Bullet* TakeBullet();
	void ReturnBullet(Bullet* bullet);
	TileMap* GetMap() { return tileMap; }
	ItemMaker* GetMaker() { return itemMaker; }
	const std::list<Zombie*>& GetZombieList() const { return activeZombies; }
	int GetNumOfZombies();
	void ScoreUp(int score);
	Status GetCurrentStatus() const { return currentStatus; }

	void OnZombieDie(Zombie* zombie);
	void OnPlayerDie();
	void OnUpgrade(int upgrade);
};

