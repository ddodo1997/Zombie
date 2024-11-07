#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "TileMap.h"
#include "Zombie.h"
#include "Bullet.h"
#include "ItemMaker.h"
#include "UiHud.h"
#include "UiUpgrade.h"
#include "UiGameOver.h"
int SceneGame::highScore = 0;
SceneGame::SceneGame()
	:Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	tileMap = AddGo(new TileMap("TileMap"));
	player = AddGo(new Player("Player"));
	itemMaker = AddGo(new ItemMaker("ItemMaker"));
	uiHud = AddGo(new UiHud("HUD"));
	uiUpgrade = AddGo(new UiUpgrade("Upgrade"));
	uiGameover = AddGo(new UiGameOver("UiGameOver"));
	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	SOUND_MGR.PlayBgm("sound/cunning_city.mp3");
	FRAMEWORK.GetWindow().setMouseCursorVisible(false);
	cursor.setTexture(TEXTURE_MGR.Get("graphics/crosshair.png"));
	Utils::SetOrigin(cursor, Origins::MC);

	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();
	FRAMEWORK.SetTimeScale(1.f);
	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	uiGameover->SetActive(false);

	SetStatus(Status::Awake);

	Scene::Enter();

	uiHud->SetCenter("Enter TO START...");
}

void SceneGame::Exit()
{
	SOUND_MGR.StopBgm();
	FRAMEWORK.GetWindow().setMouseCursorVisible(true);
	for (auto zombie : activeZombies)
	{
		RemoveGo(zombie);
		zombiePool.Return(zombie);
	}
	activeZombies.clear();

	for (auto bullet : activeBullet)
	{
		RemoveGo(bullet);
		bulletPool.Return(bullet);
	}
	activeBullet.clear();

	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	cursor.setPosition(ScreenToUi(InputMgr::GetMousePosition()));
	UpdateUI();

	if (player != nullptr)
	{
		worldView.setCenter(player->GetPosition());
	}

	switch (currentStatus)
	{
	case Status::Awake:
		UpdateAwake(dt);
		break;

	case Status::Wave:
		UpdateWave(dt);
		break;

	case Status::Interval:
		UpdateInterval(dt);
		break;

	case Status::Upgrade:
		UpdateUpgrade(dt);
		break;

	case Status::Pause:
		UpdatePause(dt);
		break;

	case Status::GameOver:
		UpdateGameOver(dt);
		break;
	}

	Scene::Update(dt);
}

void SceneGame::UpdateAwake(float dt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		player->Awake();
		SetStatus(Status::Wave);
	}
}

void SceneGame::UpdateWave(float dt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::Tab) && upgradeCount > 0)
	{
		SetStatus(Status::Upgrade);
	}
	if (itemMaker->GetFlag())
	{
		itemMaker->MakeItem(tileMap->GetGlobalBounds(), 3);
		itemMaker->SetFlag(false);
	}

	if (GetNumOfZombies() == 0)
	{
		upgradeCount++;
		SetStatus(Status::Interval);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		SetStatus(Status::Pause);
	}
}

void SceneGame::UpdateInterval(float dt)
{
	intervalTimer -= dt;	
	
	if (InputMgr::GetKeyDown(sf::Keyboard::Tab) && upgradeCount > 0)
	{
		SetStatus(Status::Upgrade);
	}
	if (intervalTimer <= IntervalDelay)
	{
		SetStatus(Status::Wave);
		intervalTimer = 10.f;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		SetStatus(Status::Pause);
	}
}

void SceneGame::UpdateUpgrade(float dt)
{
	FRAMEWORK.SetTimeScale(0.f);
	uiUpgrade->SetActive(true);
}

void SceneGame::UpdatePause(float dt)
{
	FRAMEWORK.SetTimeScale(0.f);
	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		SetStatus(prevStatus);
	}
}

void SceneGame::UpdateGameOver(float dt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		uiGameover->SetActive(false);
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}
}

void SceneGame::UpdateUI()
{
	uiHud->SetScore(score);
	uiHud->SetHiScore(highScore);
	uiHud->SetUpgrade(upgradeCount);
	uiHud->SetAmmo(player->GetAmmo(), player->GetSpareAmmo());
	uiHud->SetHp(player->GetHp(), player->GetMaxHp());
	uiHud->SetInterval((int)intervalTimer);
	uiHud->SetWave(currentWave);
	uiHud->SetZombieCount(GetNumOfZombies());
}

void SceneGame::SetStatus(Status status)
{
	auto windowSize = FRAMEWORK.GetWindowSizeF();
	prevStatus = currentStatus;
	currentStatus = status;

	if (prevStatus == Status::Upgrade || prevStatus == Status::Pause || prevStatus == Status::GameOver)
	{
		FRAMEWORK.SetTimeScale(1.f);
	}

	switch (currentStatus)
	{
	case Status::Awake:
		if (prevStatus == Status::GameOver)
		{
			uiGameover->SetActive(false);
			score = 0;
			currentWave = 0;
			upgradeCount = 0;
			intervalTimer = 10.f;
			player->Awake();
		}
		break;
	case Status::Wave:
		uiHud->SetCenter("");
		if (prevStatus != Status::Pause && prevStatus != Status::Upgrade)
		{
			currentWave++;
			SpawnZomies(20 + currentWave * currentWave);
		}
		break;
	case Status::Interval:
		uiHud->SetCenter("");
		break;
	case Status::Upgrade:
		uiHud->SetCenter("");
		break;
	case Status::Pause:
		uiHud->SetCenter("ESC TO RESTART...");
		break;
	case Status::GameOver:
		uiGameover->SetActive(true);
		FRAMEWORK.SetTimeScale(0.f);
		break;
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

	const sf::View& saveView = window.getView();
	window.setView(uiView);
	window.draw(cursor);
	window.setView(saveView);
}

void SceneGame::SpawnZomies(int count)
{
	for (int i = 0; i < count; i++)
	{
		Zombie* zombie = zombiePool.Take();
		activeZombies.push_back(zombie);

		Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(0, Zombie::TotalTypes - 1);
		zombie->SetType(zombieType);

		sf::Vector2f pos;
		pos.x = Utils::RandomRange(tileMap->GetGlobalBounds().left, tileMap->GetGlobalBounds().width + tileMap->GetGlobalBounds().left);
		pos.y = Utils::RandomRange(tileMap->GetGlobalBounds().top, tileMap->GetGlobalBounds().height + tileMap->GetGlobalBounds().top);

		zombie->SetPosition(pos);
		AddGo(zombie);
	}
}

Bullet* SceneGame::TakeBullet()
{
	Bullet* bullet = bulletPool.Take();
	activeBullet.push_back(bullet);
	AddGo(bullet);
	return bullet;
}

void SceneGame::ReturnBullet(Bullet* bullet)
{
	RemoveGo(bullet);
	bulletPool.Return(bullet);
	activeBullet.remove(bullet);
}

int SceneGame::GetNumOfZombies()
{
	int cnt = 0;
	for (auto zombie : activeZombies)
	{
		if (zombie->GetType() != Zombie::Types::Nugget)
			cnt++;
	}
	return cnt;
}

void SceneGame::ScoreUp(int score)
{
	this->score += score;
	if (this->score > highScore)
		highScore = this->score;
}

void SceneGame::OnZombieDie(Zombie* zombie)
{
	RemoveGo(zombie);
	zombiePool.Return(zombie);
	activeZombies.remove(zombie);
}



void SceneGame::OnPlayerDie()
{
	SOUND_MGR.PlaySfx("sound/splat.wav");
	SetStatus(Status::GameOver);
}

void SceneGame::OnUpgrade(int upgrade)
{
	switch ((UiUpgrade::Upgrades)upgrade)
	{
	case UiUpgrade::Upgrades::FireRate:
		player->UpgradeFireRate(0.005f);
		break;
	case UiUpgrade::Upgrades::ClipSize:
		player->UpgradeClipSize(2);
		break;
	case UiUpgrade::Upgrades::MaxHp:
		player->UpgradeMaxHp(20);
		break;
	case UiUpgrade::Upgrades::RunSpeed:
		player->UpgradeSpeed(25.f);
		break;
	case UiUpgrade::Upgrades::MoreHealth:
		player->UpgradeHealing(2);
		break;
	case UiUpgrade::Upgrades::MoreAmmo:
		player->UpgradeBullets(2);
		break;
	}
	SOUND_MGR.PlaySfx("sound/powerup.wav");
	upgradeCount--;
	SetStatus(prevStatus);
	uiUpgrade->SetActive(false);
}