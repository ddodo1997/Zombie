#include "stdafx.h"
#include "SceneGame.h"
#include "ItemMaker.h"
#include "Item.h"
ItemMaker::ItemMaker(const std::string& name)
	: GameObject(name)
{
}

void ItemMaker::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void ItemMaker::SetRotation(float angle)
{
	rotation = angle;
}

void ItemMaker::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void ItemMaker::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void ItemMaker::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void ItemMaker::Init()
{
}

void ItemMaker::Release()
{
	for (auto item : activeItems)
	{
		itemPool.Return(item);
	}
	activeItems.clear();
}

void ItemMaker::Reset()
{
	for (auto item : activeItems)
	{
		itemPool.Return(item);
	}
	activeItems.clear();

	makeTimer = 0.f;
	makeFlag = false;
}

void ItemMaker::Update(float dt)
{
	makeTimer += dt;
	if (makeTimer > makeDelay)
	{
		makeFlag = true;
		makeTimer = 0.f;
	}
}

void ItemMaker::Draw(sf::RenderWindow& window)
{
	for (auto item : activeItems)
	{
		item->Draw(window);
	}
}

void ItemMaker::MakeItem(const sf::FloatRect& area, int count)
{
	for (int i = 0; i < count; i++)
	{
		Item* item = itemPool.Take();
		activeItems.push_back(item);
		Item::Types itemType = (Item::Types)Utils::RandomRange(0, Item::TotalItem - 1);
		item->SetType(itemType);

		sf::Vector2f pos;
		pos.x = Utils::RandomRange(area.left, area.left + area.width);
		pos.y = Utils::RandomRange(area.top, area.top + area.height);
		item->SetPosition(pos);
	}
}

void ItemMaker::OnCatchItem(Item* item)
{
	itemPool.Return(item);
	activeItems.remove(item);
}
