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
	SetPosition({ 0.f,0.f });
	SetOrigin(Origins::MC);
	itemMaker = sceneGame->GetMaker();
	TileMap* map = sceneGame->GetMap();
	sf::FloatRect mapBounds = map->GetGlobalBounds();
	movalbeArea = { 
		mapBounds.left + map->GetCellSize().x,
		mapBounds.top + map->GetCellSize().y, 
		mapBounds.width - map->GetCellSize().x - map->GetCellSize().y,
		mapBounds.height - map->GetCellSize().x - map->GetCellSize().y
	};
	ammo = maxAmmo;
	hp = maxHp;
	isAlive = true;
	reloadingBar.setFillColor(sf::Color::Green);
	reloadingBar.setSize({ 0.f,0.f });
}

void Player::Update(float dt)
{
	if (!isAlive)
		return;

	direction.x = InputMgr::GetAxis(Axis::Horizontal);
	direction.y = InputMgr::GetAxis(Axis::Vertical);
	float dirMagnitude = Utils::Magnitude(direction);
	if (dirMagnitude > 1.f)
	{
		Utils::Normailize(direction);
	}
	auto newPos = position + direction * speed * dt;

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
		reloadTimer += dt;
		float value = (float)reloadTimer / reloadDelay;
		if (value < 0.f)
		{
			value = 0.f;
		}
		reloadingBar.setSize({ reloadingBarSize.x * value , reloadingBarSize.y});
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
		if(ammo != 6)
			isReloading = true;
	}

	shootTimer += dt;
	if (shootTimer > shootDelay && InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		if(!Shoot())
		{
			shootTimer = 0.f;
		}
		else
		{
			isReloading = true;
		}
	}
}

void Player::FixedUpdate(float dt)
{
	if (sceneGame == nullptr || !isAlive)
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
			Item::Types type = item->GetType();
			switch (type)
			{
			case Item::Types::Ammo:
				spareAmmo += pickAmmo;
				if (spareAmmo > 99)
					spareAmmo = 99;
				break;
			case Item::Types::AID:
				hp += healHp;
				if (hp > maxHp)
					hp = maxHp;
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
	if (isReloading == true)
	{
		return needReloading;
	}
	if (ammo <= 0)
	{
		needReloading = true;
		return needReloading;
	}
	ammo -= 1;
	Bullet* bullet = sceneGame->TakeBullet();
	bullet->Fire(position, look, 1000.f, 10);
	return needReloading;
}

void Player::Reloading()
{
	if (spareAmmo > 0)
	{
		if (spareAmmo < maxAmmo)
		{
			int tempAmmo = maxAmmo - ammo;
			ammo += spareAmmo;
			if (ammo > maxAmmo)
			{
				ammo = maxAmmo;
				spareAmmo -= tempAmmo;
				if (spareAmmo < 0)
					spareAmmo = 0;		//...? 코드에 찔려 죽겠는데
			}
			else
			{
				spareAmmo = 0;
			}
		}
		else
		{
			spareAmmo += ammo - maxAmmo;
			ammo = maxAmmo;
		}
		needReloading = false;
		isReloading = false;
		return;
	}
	//TODO : 장전 할 탄약이 부족할 경우
	return;
}

void Player::OnDamege(int damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		isAlive = false;
		sceneGame->OnPlayerDie();
	}
}

