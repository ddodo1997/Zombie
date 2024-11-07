#include "stdafx.h"
#include "Item.h"

Item::Item(const std::string& name)
	: GameObject(name)
{
}

void Item::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Item::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Item::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Item::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Item::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Item::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 1;
	SetType(type);
	SetOrigin(Origins::MC);
}

void Item::Release()
{
}

void Item::Reset()
{
}

void Item::Update(float dt)
{
}

void Item::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void Item::SetType(Types type)
{
	this->type = type;
	switch (this->type)
	{
	case Types::Ammo:
		texItemId = "graphics/ammo_pickup.png";
		break;
	case Types::AID:
		texItemId = "graphics/health_pickup.png";
		break;
	case Types::AssaultRifle:
		texItemId = "graphics/assault_rifle.png";
		body.setScale({ 0.1f,0.1f });
		break;
	case Types::ShotGun:
		texItemId = "graphics/shotgun.png";
		break;
	}
	body.setTexture(TEXTURE_MGR.Get(texItemId), true);
}
