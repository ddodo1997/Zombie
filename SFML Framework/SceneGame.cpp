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
	FRAMEWORK.GetWindow().setMouseCursorVisible(false);
	cursor.setTexture(TEXTURE_MGR.Get("graphics/crosshair.png"));
	Utils::SetOrigin(cursor, Origins::MC);

	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();
	FRAMEWORK.SetTimeScale(1.f);
	worldView.setSize(FRAMEWORK.GetWindowSizeF());
	worldView.setCenter(0.f, 0.f);

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	uiGameover->SetActive(false);
	Scene::Enter();
}

void SceneGame::Exit()
{

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
	uiHud->SetScore(0);
	uiHud->SetHiScore(0);
	uiHud->SetAmmo(player->GetAmmo(), player->GetSpareAmmo());
	uiHud->SetHp(player->GetHp(), player->GetMaxHp());
	uiHud->SetWave(0);
	uiHud->SetZombieCount(0);


	if (player != nullptr)
	{
		worldView.setCenter(player->GetPosition());
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		SpawnZomies(20);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		itemMaker->SetActive(!itemMaker->IsActive());
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		uiUpgrade->SetActive(!uiUpgrade->IsActive());
	}

	if (itemMaker->GetFlag())
	{
		itemMaker->MakeItem(tileMap->GetGlobalBounds(), 5);
		itemMaker->SetFlag(false);
	}

	Scene::Update(dt);
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

void SceneGame::OnZombieDie(Zombie* zombie)
{
	RemoveGo(zombie);
	zombiePool.Return(zombie);
	activeZombies.remove(zombie);
}

void SceneGame::OnPlayerDie()
{
	uiGameover->SetActive(true);
	FRAMEWORK.SetTimeScale(0.f);
}

void SceneGame::OnUpgrade(sf::Text& text, int upgrade)
{
	switch ((UiUpgrade::Upgrades)upgrade)
	{
	case UiUpgrade::Upgrades::FireRate:
		player->UpgradeFireRate(10.f);
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
	uiUpgrade->SetActive(false);
}
