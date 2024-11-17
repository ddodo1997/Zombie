#include "stdafx.h"
#include "UiHud.h"

UiHud::UiHud(const std::string& name)
	: GameObject(name)
{
}

void UiHud::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiHud::SetRotation(float angle)
{
	rotation = angle;
}

void UiHud::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiHud::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiHud::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiHud::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 0;
}

void UiHud::Release()
{
}

void UiHud::Reset()
{
	float textSize = 50.f;
	sf::Font& font = FONT_MGR.Get("fonts/H2HDRM.TTF");
	//상단
	textScore.setFont(font);
	textScore.setCharacterSize(textSize);
	textScore.setFillColor(sf::Color::White);
	Utils::SetOrigin(textScore, Origins::TL);
	textHighScore.setFont(font);
	textHighScore.setCharacterSize(textSize);
	textHighScore.setFillColor(sf::Color::White);
	Utils::SetOrigin(textHighScore, Origins::TR);
	textUpgrade.setFont(font);
	textUpgrade.setCharacterSize(textSize * 1.5f);
	textUpgrade.setFillColor(sf::Color::Red);

	//중앙
	textCenter.setFont(font);
	textCenter.setCharacterSize(textSize * 4.f);
	textCenter.setFillColor(sf::Color::Red);
	Utils::SetOrigin(textCenter, Origins::MC);

	//하단
	Utils::SetOrigin(textUpgrade, Origins::TC);
	textAmmo.setFont(font);
	textAmmo.setCharacterSize(textSize);
	textAmmo.setFillColor(sf::Color::White);
	Utils::SetOrigin(textAmmo, Origins::BL);
	textInterval.setFont(font);
	textInterval.setCharacterSize(textSize * 2);
	textInterval.setFillColor(sf::Color::Red);
	Utils::SetOrigin(textInterval, Origins::BC);
	textWave.setFont(font);
	textWave.setCharacterSize(textSize);
	textWave.setFillColor(sf::Color::White);
	Utils::SetOrigin(textWave, Origins::BR);
	textZombieCount.setFont(font);
	textZombieCount.setCharacterSize(textSize);
	textZombieCount.setFillColor(sf::Color::White);
	Utils::SetOrigin(textZombieCount, Origins::BR);

	gaugeHp.setFillColor(sf::Color::Red);
	gaugeHp.setSize(gaugeHpMaxSize);
	Utils::SetOrigin(gaugeHp, Origins::ML);

	iconAmmo.setTexture(TEXTURE_MGR.Get("graphics/ammo_icon.png"));
	Utils::SetOrigin(iconAmmo, Origins::BL);

	float topY = 25.f;
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();
	float bottomY = size.y - 50.f;

	textScore.setPosition(25.f, topY);
	textHighScore.setPosition(size.x - 25.f, topY);
	textUpgrade.setPosition(size.x * 0.5f, topY);

	textCenter.setPosition(size.x * 0.5f, size.y * 0.5f);

	iconAmmo.setPosition(25.f, bottomY);
	textAmmo.setPosition(80.f, bottomY);
	gaugeHp.setPosition(300.f, bottomY);
	textInterval.setPosition(size.x * 0.5f, size.y - 200);
	textWave.setPosition(size.x - 400.f, bottomY);
	textZombieCount.setPosition(size.x - 25.f, bottomY);
}

void UiHud::Update(float dt)
{
}

void UiHud::Draw(sf::RenderWindow& window)
{
	window.draw(textScore);
	window.draw(textUpgrade);
	window.draw(textHighScore);
	window.draw(textCenter);
	window.draw(iconAmmo);
	window.draw(textAmmo);
	window.draw(gaugeHp);
	window.draw(textInterval);
	window.draw(textWave);
	window.draw(textZombieCount);
}

void UiHud::SetScore(int s)
{
	textScore.setString(STRING_TABLE->Get("Score") + std::to_wstring(s));
	Utils::SetOrigin(textScore, Origins::TL);
}

void UiHud::SetHiScore(int s)
{
	textHighScore.setString(STRING_TABLE->Get("HighScore") + std::to_wstring(s));
	Utils::SetOrigin(textHighScore, Origins::TR);
}

void UiHud::SetCenter(std::wstring str)
{
	textCenter.setString(str);
	Utils::SetOrigin(textCenter, Origins::MC);
}

void UiHud::SetUpgrade(int u)
{
	if (u == 0)
	{
		textUpgrade.setString("");
		return;
	}
	textUpgrade.setString(STRING_TABLE->Get("UpgradeAlert"));
	Utils::SetOrigin(textUpgrade, Origins::TC);
}

void UiHud::SetInterval(int i)
{
	if(i >= 10)
	{
		textInterval.setString("");
		return;
	}
	textInterval.setString(STRING_TABLE->Get("Interval") + std::to_wstring(i));
	Utils::SetOrigin(textInterval, Origins::BC);
}

void UiHud::SetAmmo(int current, int total)
{
	textAmmo.setString(std::to_string(current) + " / " + std::to_string(total));
	Utils::SetOrigin(textAmmo, Origins::BL);
}

void UiHud::SetHp(int hp, int max)
{
	float value = (float)hp / max;
	if (value < 0.f)
	{
		value = 0.f;
	}
	gaugeHp.setSize({ gaugeHpMaxSize.x * value, gaugeHpMaxSize.y });

	Utils::SetOrigin(gaugeHp, Origins::ML);
}

void UiHud::SetWave(int w)
{
	textWave.setString(STRING_TABLE->Get("Wave") + std::to_wstring(w));
	Utils::SetOrigin(textWave, Origins::BR);
}

void UiHud::SetZombieCount(int count)
{
	textZombieCount.setString(STRING_TABLE->Get("Zombies") + std::to_wstring(count));
	Utils::SetOrigin(textZombieCount, Origins::BR);
}
