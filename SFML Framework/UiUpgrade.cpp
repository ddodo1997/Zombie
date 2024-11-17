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
	int textSize = 100;
	sf::Vector2f textPos({ 200.f, 250.f });
	for (int i = 0; i < TotalUpgrades; i++)
	{
		textUpgrades[i].setFont(FONT_MGR.Get("fonts/H2HDRM.TTF"));
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
		
		if (InputMgr::GetMouseButton(sf::Mouse::Right))
		{
			if (Utils::PointInTransformBounds(textUpgrades[i], textUpgrades[i].getLocalBounds(), mousePos))
			{
				sceneGame->OnUpgrade(i);
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

void UiUpgrade::OnLocallize(Languages lang)
{
	GameObject::OnLocallize(lang);
	Reset();
}

void UiUpgrade::SetText(sf::Text& text, Upgrades upgrade)
{
	switch (upgrade)
	{
	case Upgrades::FireRate:
		text.setString(STRING_TABLE->Get("UpgradeRateOfFire"));
		break;
	case Upgrades::ClipSize:
		text.setString(STRING_TABLE->Get("UpgradeClipSize"));
		break;
	case Upgrades::MaxHp:
		text.setString(STRING_TABLE->Get("UpgradeMaxHP"));
		break;
	case Upgrades::RunSpeed:
		text.setString(STRING_TABLE->Get("UpgradeRunSpeed"));
		break;
	case Upgrades::MoreHealth:
		text.setString(STRING_TABLE->Get("UpgradeHPPickUp"));
		break;
	case Upgrades::MoreAmmo:
		text.setString(STRING_TABLE->Get("UpgradeAmmoPickUp"));
		break;
	}
	Utils::SetOrigin(text, Origins::TL);
}
