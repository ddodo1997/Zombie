#include "stdafx.h"
#include "UiGameOver.h"

UiGameOver::UiGameOver(const std::string& name)
	: GameObject(name)
{
}

void UiGameOver::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiGameOver::SetRotation(float angle)
{
	rotation = angle;
}

void UiGameOver::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiGameOver::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiGameOver::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiGameOver::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 0;
}

void UiGameOver::Release()
{
}

void UiGameOver::Reset()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
	textGameover.setFont(FONT_MGR.Get("fonts/H2HDRM.TTF"));
	textGameover.setCharacterSize(250.f);
	textGameover.setFillColor(sf::Color::Red);
	textGameover.setString(STRING_TABLE->Get("GameOver"));
	Utils::SetOrigin(textGameover, Origins::MC);
	textGameover.setPosition({windowSize.x * 0.5f, windowSize.y * 0.5f });
	SetActive(false);
}

void UiGameOver::Update(float dt)
{
}

void UiGameOver::Draw(sf::RenderWindow& window)
{
	window.draw(textGameover);
}
