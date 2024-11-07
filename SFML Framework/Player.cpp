#include "stdafx.h"
#include "Player.h"
#include "SceneGame.h"
#include "Bullet.h"
#include "TileMap.h"
#include "ItemMaker.h"
#include "Item.h"
#include "Zombie.h"
Player::Player(const std::string& name)
	: GameObject(name)
{
}

void Player::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
	collisionBox.setPosition(position);
}

void Player::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
	collisionBox.setRotation(rotation);
}

void Player::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
	collisionBox.setScale(scale);
}

void Player::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
		Utils::SetOrigin(collisionBox, originPreset);
	}
}

void Player::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
	collisionBox.setOrigin(origin);
}

void Player::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
}

void Player::Release()
{

}

void Player::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	body.setTexture(TEXTURE_MGR.Get(texPlayerId));
	collisionBox.setSize({ body.getLocalBounds().width, body.getLocalBounds().height });
	itemMaker = sceneGame->GetMaker();
	TileMap* map = sceneGame->GetMap();
	sf::FloatRect mapBounds = map->GetGlobalBounds();
	movalbeArea = {
		mapBounds.left + map->GetCellSize().x,
		mapBounds.top + map->GetCellSize().y,
		mapBounds.width - map->GetCellSize().x - map->GetCellSize().y,
		mapBounds.height - map->GetCellSize().x - map->GetCellSize().y
	};

	reloadingBar.setFillColor(sf::Color::Green);
	reloadingBar.setSize({ 0.f,0.f });
	reloadingBar.setOutlineColor(sf::Color::Transparent);
	reloadingBar.setOutlineThickness(2.f);

	Awake();
}

void Player::Update(float dt)
{
	if (!isAlive || sceneGame->GetCurrentStatus() == SceneGame::Status::Pause)
		return;

	direction.x = InputMgr::GetAxis(Axis::Horizontal);
	direction.y = InputMgr::GetAxis(Axis::Vertical);
	float dirMagnitude = Utils::Magnitude(direction);
	if (dirMagnitude > 1.f)
	{
		Utils::Normailize(direction);
	}
	auto newPos = position + direction * (speed + upgradeRunSpeed) * dt;

	if (newPos.x < movalbeArea.left)
	{
		newPos.x = movalbeArea.left;
	}
	if (newPos.x > movalbeArea.width + movalbeArea.left)
	{
		newPos.x = movalbeArea.width + movalbeArea.left;
	}

	if (newPos.y < movalbeArea.top)
	{
		newPos.y = movalbeArea.top;
	}
	if (newPos.y > movalbeArea.height + movalbeArea.top)
	{
		newPos.y = movalbeArea.height + movalbeArea.top;
	}

	SetPosition(newPos);
	reloadingBar.setPosition({ newPos.x - GetLocalBounds().width * 0.7f, newPos.y - GetLocalBounds().height });

	if (isReloading)
	{
		reloadingBar.setOutlineColor(sf::Color::Black);

		reloadTimer += dt;
		float value = (float)reloadTimer / reloadDelay;
		if (value < 0.f)
		{
			value = 0.f;
		}
		reloadingBar.setSize({ reloadingBarSize.x * value , reloadingBarSize.y });
		if (reloadDelay <= reloadTimer)
		{
			Reloading();
			reloadTimer = 0.f;
			reloadingBar.setSize({ 0.f,0.f });
		}
	}

	sf::Vector2i mousePos = (sf::Vector2i)InputMgr::GetMousePosition();
	sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(mousePos);
	look = Utils::GetNormal(mouseWorldPos - position);
	SetRotation(Utils::Angle(look));

	if (InputMgr::GetKeyDown(sf::Keyboard::R))
	{
		if (ammo < maxAmmo + upgradeClipSize && spareAmmo > 0)
			isReloading = true;
	}

	shootTimer += dt;
	if (shootTimer > shootDelay - upgradeFireRate && InputMgr::GetMouseButton(sf::Mouse::Left))
	{
		if (!Shoot())
		{
			shootTimer = 0.f;
		}
		else
		{
			SOUND_MGR.PlaySfx("sound/reload.wav");
			isReloading = true;
		}
	}
}

void Player::SetWeapon(Weapon weapon)
{
	prevWeapon = currentWeapon;
	currentWeapon = weapon;

	switch (currentWeapon)
	{
	case Weapon::Pistol:
		speed = 500.f;
		reloadDelay = 2.f;
		shootDelay = 0.5f;
		maxAmmo = 6;
		spareAmmo = 10;
		ammo = maxAmmo + upgradeClipSize;
		break;

	case Weapon::Assault:
		speed = 700.f;
		reloadDelay = 4.f;
		shootDelay = 0.05f;
		maxAmmo = 60;
		spareAmmo = 100;
		ammo = maxAmmo + upgradeClipSize;
		break;

	case Weapon::ShotGun:
		speed = 400.f;
		reloadDelay = 5.f;
		shootDelay = 1.f;
		maxAmmo = 2;
		spareAmmo = 5;
		ammo = maxAmmo + upgradeClipSize;
		break;
	}
	reloadTimer = reloadDelay;
}

void Player::FixedUpdate(float dt)
{
	if (sceneGame == nullptr || !isAlive || sceneGame->GetCurrentStatus() == SceneGame::Status::Pause)
		return;

	const auto& list = itemMaker->GetActiveItems();

	for (auto item : list)
	{
		if (!item->IsActive())
			continue;

		sf::FloatRect itemBounds = item->GetGlobalBounds();
		sf::FloatRect bounds = GetGlobalBounds();
		if (bounds.intersects(itemBounds))
		{

			SOUND_MGR.PlaySfx("sound/pickup.wav");
			Item::Types type = item->GetType();
			switch (type)
			{
			case Item::Types::Ammo:
				spareAmmo += pickAmmo + upgradePickAmmo;
				if (spareAmmo > 500)
					spareAmmo = 500;
				break;
			case Item::Types::AID:
				hp += healHp + upgradeHealing;
				if (hp > maxHp + upgradeMaxHp)
					hp = maxHp + upgradeMaxHp;
				break;
			case Item::Types::AssaultRifle:
				SetWeapon(Weapon::Assault);
				break;
			case Item::Types::ShotGun:
				SetWeapon(Weapon::ShotGun);
				break;
			}
			itemMaker->OnCatchItem(item);
			break;
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	window.draw(reloadingBar);
}

bool Player::Shoot()
{

	switch (currentWeapon)
	{
	case Weapon::Pistol:
		needReloading = ShootPistol();
		break;
	case Weapon::Assault:
		needReloading = ShootAssault();
		break;
	case Weapon::ShotGun:
		needReloading = ShootShotGun();
		break;
	}
	return needReloading;
}

bool Player::ShootPistol()
{
	if (isReloading == true)
	{
		return needReloading;
	}
	if (ammo <= 0)
	{
		if (spareAmmo <= 0)
			return needReloading;

		needReloading = true;
		return needReloading;
	}
	ammo -= 1;
	Bullet* bullet = sceneGame->TakeBullet();
	bullet->Fire(position, look, 1000.f, 10);
	return needReloading;
}

bool Player::ShootAssault()
{
	if (isReloading == true)
	{
		return needReloading;
	}
	if (ammo <= 0)
	{
		if (spareAmmo <= 0)
			return needReloading;

		needReloading = true;
		return needReloading;
	}
	ammo -= 1;
	Bullet* bullet = sceneGame->TakeBullet();
	auto rand = Utils::RandomRange(0.f, 1.f);
	bullet->Fire(position, { look.x * rand, look.y }, 1000.f, 7);
	return needReloading;
}

bool Player::ShootShotGun()
{
	if (isReloading == true)
	{
		return needReloading;
	}
	if (ammo <= 0)
	{
		if (spareAmmo <= 0)
			return needReloading;

		needReloading = true;
		return needReloading;
	}
	ammo -= 1;
	for (int i = 0; i < 13; i++)
	{
		Bullet* bullet = sceneGame->TakeBullet();
		auto rand1 = Utils::RandomRange(0.5f, 1.f);
		auto rand2 = Utils::RandomRange(0.7f, 0.8f);
		bullet->Fire(position, { look.x * rand1, look.y * rand2 }, 1000.f, 10);
	}
	return needReloading;
}

void Player::Reloading()
{
	if (spareAmmo > 0)
	{
		if (spareAmmo < maxAmmo + upgradeClipSize)
		{
			int tempAmmo = maxAmmo + upgradeClipSize - ammo;
			ammo += spareAmmo;
			if (ammo > maxAmmo + upgradeClipSize)
			{
				ammo = maxAmmo + upgradeClipSize;
				spareAmmo -= tempAmmo;
			}
			else
			{
				spareAmmo = 0;
			}
		}
		else
		{
			spareAmmo += ammo - maxAmmo + upgradeClipSize;
			ammo = maxAmmo + upgradeClipSize;
		}
		needReloading = false;
		isReloading = false;
		return;
	}
	//TODO : 장전 할 탄약이 부족할 경우
	SOUND_MGR.PlaySfx("sound/reload_failed.wav");
}

void Player::OnDamege(int damage)
{
	SOUND_MGR.PlaySfx("sound/hit.wav");
	hp -= damage;
	if (hp <= 0)
	{
		isAlive = false;
		sceneGame->OnPlayerDie();
	}
}

void Player::Awake()
{
	SetPosition({ 0.f,0.f });
	SetOrigin(Origins::MC);
	SetWeapon(Weapon::Pistol);
	reloadTimer = 0.f;
	maxHp = 50;
	hp = maxHp + upgradeMaxHp;
	isAlive = true;

	upgradeFireRate = 0.f;
	upgradeClipSize = 0;
	upgradeMaxHp = 0;
	upgradeRunSpeed = 0.f;
	upgradeHealing = 0;
	upgradePickAmmo = 0;
}

