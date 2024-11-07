#include "stdafx.h"
#include "Bullet.h"
#include "SceneGame.h"
#include "Zombie.h"

Bullet::Bullet(const std::string& name)
	: GameObject(name)
{
}

void Bullet::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
	collisionBox.setPosition(position);
}

void Bullet::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Bullet::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
	collisionBox.setScale(scale);
}

void Bullet::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
		Utils::SetOrigin(collisionBox, originPreset);
	}
}

void Bullet::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
	collisionBox.setOrigin(origin);
}

void Bullet::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 1;
}

void Bullet::Release()
{
}

void Bullet::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	body.setTexture(TEXTURE_MGR.Get(texBulletId));
	collisionBox.setSize({ body.getLocalBounds().width, body.getLocalBounds().height });

	SetOrigin(Origins::ML);
	SetPosition({ 0.f,0.f });
	SetScale({ 2.f,2.f });
	SetRotation(0.f);
}

void Bullet::Update(float dt)
{
	SetPosition(position + direction * speed * dt);
	activeTimer += dt;
	if (activeDelay < activeTimer)
	{
		sceneGame->ReturnBullet(this);
		activeTimer = 0.f;
	}
}

void Bullet::FixedUpdate(float dt)
{
	if (sceneGame == nullptr)
		return;

	const auto& list = sceneGame->GetZombieList();

	for (auto zombie : list)
	{
		if (!zombie->IsActive() || zombie->GetType() == Zombie::Types::Nugget)
			continue;

		sf::FloatRect bounds = GetGlobalBounds();
		sf::FloatRect zombieBounds = zombie->GetGlobalBounds();
		if (bounds.intersects(zombieBounds)&& Utils::CheckCollision(collisionBox, zombie->GetCollisionBox()))
		{
				// 1. 불릿에서 좀비에게 데미지를 줄것
				// 2. 좀비에서 데미지만큼 체력을 뺄것
				// 3. 좀비의 체력이 모두 닳았다면, 
				// 씬에게 좀비가 죽었으니 remove하도록 시킬것
				//debugBox.SetOutlineColor(sf::Color::Red);
				zombie->OnDamage(damage);
				sceneGame->ReturnBullet(this);
				break;
		}
	}
}

void Bullet::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void Bullet::Fire(const sf::Vector2f& pos, const sf::Vector2f& direction, float speed, int damage)
{
	SOUND_MGR.PlaySfx("sound/shoot.wav");
	SetPosition(pos);
	this->direction = direction;
	this->speed = speed;
	this->damage = damage;
	SetRotation(Utils::Angle(direction));
}
