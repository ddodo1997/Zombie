#include "stdafx.h"
#include "Zombie.h"
#include "Player.h"
#include "SceneGame.h"
#include "ZombieTable.h"
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
	hpBar.setFillColor(sf::Color::Green);
	hpBar.setSize(hpBarSize);
	hpBar.setOutlineColor(sf::Color::Black);
	hpBar.setOutlineThickness(2.f);
}

void Zombie::Update(float dt)
{
	if (isNugget)
	{
		hpBar.setOutlineColor(sf::Color::Transparent);
		nuggetTimer += dt;
		if (nuggetDelay < nuggetTimer)
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

	auto newPos = position + direction * speed * dt;
	hpBar.setPosition({ newPos.x - GetLocalBounds().width * 0.7f, newPos.y - GetLocalBounds().height });
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
	window.draw(hpBar);
}

void Zombie::SetType(Types types)
{
	this->types = types;
	const auto& data = ZOMBIE_TABLE->Get(this->types);
	texZombieId = data.texZombieId;
	maxHp = data.maxHp;
	attack = data.attack;
	attackDelay = data.attackDelay;
	speed = data.speed;

	body.setTexture(TEXTURE_MGR.Get(texZombieId), true);
	hp = maxHp;
}

void Zombie::OnDamage(int damage)
{
	hp -= damage;
	float value = (float)hp / maxHp;
	if (value < 0.f)
	{
		value = 0.f;
	}
	hpBar.setSize({ hpBarSize.x * value , hpBarSize.y });

	if (hp <= 0 && sceneGame != nullptr)
	{
		isNugget = true;
		SOUND_MGR.PlaySfx("sound/splat.wav");
		switch (types)
		{
		case Types::Bloater:
			sceneGame->ScoreUp(500);
			break;
		case Types::Chaser:
			sceneGame->ScoreUp(200);
			break;
		case Types::Crawler:
			sceneGame->ScoreUp(100);
			break;
		}
		SetType(Types::Nugget);
	}
}

SaveZombie Zombie::GetSaveData() const
{
	return SaveZombie{(int)types,position,rotation,scale,hp};
}

void Zombie::LoadSaveData(const SaveZombie& data)
{
	SetType((Types)data.type);
	SetPosition(data.position);
	SetRotation(data.rotation);
	SetScale(data.scale);
	hp = data.hp;
}

