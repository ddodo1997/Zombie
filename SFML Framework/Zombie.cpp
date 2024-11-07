#include "stdafx.h"
#include "Zombie.h"
#include "Player.h"
#include "SceneGame.h"
Zombie::Zombie(const std::string& name)
	: GameObject(name)
{
}

void Zombie::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
	collisionBox.setPosition(position);
}

void Zombie::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
	collisionBox.setRotation(rotation);
}

void Zombie::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
	collisionBox.setScale(scale);
}

void Zombie::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
		Utils::SetOrigin(collisionBox, originPreset);
	}
}

void Zombie::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
	collisionBox.setOrigin(origin);
}

void Zombie::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	SetType(types);
	isNugget = false;
}

void Zombie::Release()
{
}

void Zombie::Reset()
{
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	body.setTexture(TEXTURE_MGR.Get(texZombieId));
	collisionBox.setSize({ body.getLocalBounds().width, body.getLocalBounds().height });
	SetOrigin(Origins::MC);
	SetPosition({ 0.f,0.f });
	SetRotation(0.f);
	SetScale({ 1.f,1.f });
	types = Types::Bloater;
	SetType(types);
	debugBox.SetOutlineColor(sf::Color::Green);
}

void Zombie::Update(float dt)
{
	if (isNugget)
	{
		nuggetTimer += dt;
		if(nuggetDelay < nuggetTimer)
		{
			sceneGame->OnZombieDie(this);
			nuggetTimer = 0;
			isNugget = false;
		}
		return;
	}

	if (player != nullptr && Utils::Distance(position, player->GetPosition()) > 30)
	{
		direction = Utils::GetNormal(player->GetPosition() - position);
		SetRotation(Utils::Angle(direction));
		SetPosition(position + direction * speed * dt);
	}
	debugBox.SetBounds(GetGlobalBounds());
}

void Zombie::FixedUpdate(float dt)
{
	if (sceneGame == nullptr || isNugget)
		return;

	sf::FloatRect playerBounds = player->GetGlobalBounds();
	sf::FloatRect bounds = GetGlobalBounds();
	attackTimer += dt;
	if (bounds.intersects(playerBounds) && attackTimer >= attackDelay)
	{
		if (Utils::CheckCollision(collisionBox, player->GetCollisionBox()))
		{
			player->OnDamege(attack);
			attackTimer = 0.f;
		}
	}
}

void Zombie::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	debugBox.Draw(window);
}

void Zombie::SetType(Types types)
{
	this->types = types;
	switch (this->types)
	{
	case Types::Bloater:
		texZombieId = "graphics/bloater.png";
		maxHp = 50;
		attack = 10;
		attackDelay = 2.f;
		speed = 150.f;
		break;
	case Types::Chaser:
		texZombieId = "graphics/chaser.png";
		maxHp = 20;
		attack = 5;
		attackDelay = 0.5f;
		speed = 300.f;
		break;
	case Types::Crawler:
		texZombieId = "graphics/crawler.png";
		maxHp = 10;
		attack = 30;
		attackDelay = 3.f;
		speed = 50.f;
		break;
	case Types::Nugget:
		texZombieId = "graphics/blood.png";
		maxHp = 0;
		attack = 0;
		attackDelay = 0.f;
		speed = 0.f;
		break;
	}
	body.setTexture(TEXTURE_MGR.Get(texZombieId), true);
	hp = maxHp;
}

void Zombie::OnDamage(int damage)
{
	hp -= damage;
	if (hp <= 0 && sceneGame != nullptr)
	{
		isNugget = true;
		SetType(Types::Nugget);
	}
}

