#include "stdafx.h"
#include "UiUpgrade.h"
#include "SceneGame.h"
UiUpgrade::UiUpgrade(const std::string& name)
	: GameObject(name)
{
}

void UiUpgrade::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiUpgrade::SetRotation(float angle)
{
	rotation = angle;
}

void UiUpgrade::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiUpgrade::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiUpgrade::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiUpgrade::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;
	for (int i = 0; i < TotalUpgrades; i++)
	{
		sf::Text text;
		textUpgrades.push_back(text);
	}
}

void UiUpgrade::Release()
{
}

void UiUpgrade::Reset()
{
	backGround.setTexture(TEXTURE_MGR.Get("graphics/background.png"));
	float textSize = 100.f;
	sf::Vector2f textPos({ 200.f, 250.f });
	for (int i = 0; i < TotalUpgrades; i++)
	{
		textUpgrades[i].setFont(FONT_MGR.Get("fonts/zombiecontrol.ttf"));
		textUpgrades[i].setCharacterSize(textSize);
		textUpgrades[i].setFillColor(sf::Color::White);
		textUpgrades[i].setPosition({ textPos.x, textPos.y + 110 * i });
		SetText(textUpgrades[i], (Upgrades)i);
	}
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	SetActive(false);
}

void UiUpgrade::Update(float dt)
{
}

void UiUpgrade::FixedUpdate(float dt)
{
	if (sceneGame == nullptr)
		return;

	sf::Vector2f mousePos = sceneGame->ScreenToUi(InputMgr::GetMousePosition());


	for (int i = 0; i < TotalUpgrades; i++)
	{
		if (textUpgrades[i].getGlobalBounds().contains(mousePos))
		{
			textUpgrades[i].setFillColor(sf::Color::Red);
		}
		else
		{
			textUpgrades[i].setFillColor(sf::Color::White);
		}
		
		if (InputMgr::GetMouseButton(sf::Mouse::Left))
		{
			if (Utils::PointInTransformBounds(textUpgrades[i], textUpgrades[i].getLocalBounds(), mousePos))
			{
				sceneGame->OnUpgrade(textUpgrades[i], i);
			}
		}
	}
}

void UiUpgrade::Draw(sf::RenderWindow& window)
{
	window.draw(backGround);
	for (int i = 0; i < TotalUpgrades; i++)
	{
		window.draw(textUpgrades[i]);
	}
}

void UiUpgrade::SetText(sf::Text& text, Upgrades upgrade)
{
	switch (upgrade)
	{
	case Upgrades::FireRate:
		text.setString("1- INCREASED RATE OF FIRE");
		break;
	case Upgrades::ClipSize:
		text.setString("2- INCREASED CLIP SIZE (NEXT RELOAD)");
		break;
	case Upgrades::MaxHp:
		text.setString("3- INCREASED MAX HEALTH");
		break;
	case Upgrades::RunSpeed:
		text.setString("4- INCREASED RUN SPEED");
		break;
	case Upgrades::MoreHealth:
		text.setString("5- MORE AND BETTER HEALTH PICKUPS");
		break;
	case Upgrades::MoreAmmo:
		text.setString("6- MORE AND BETTER AMMO PICKUPS");
		break;
	}
	Utils::SetOrigin(text, Origins::TL);
}
