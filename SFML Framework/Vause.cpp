#include "stdafx.h"
#include "Vause.h"

Vause::Vause(const std::string& name)
	: GameObject(name)
{
}

void Vause::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Vause::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Vause::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Vause::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Vause::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Vause::Init()
{
	animator.SetTarget(&body);
	SetOrigin(Origins::TC);
	SetScale({ 2.f,2.f });
}

void Vause::Release()
{
}

void Vause::Reset()
{
	animator.Play("animations/vause_spawn.csv");
}

void Vause::Update(float dt)
{
	direction.x = InputMgr::GetAxis(Axis::Horizontal);
	SetPosition(position + direction * speed * dt);

	//Test
	if(InputMgr::GetKeyDown(sf::Keyboard::Num1))
		animator.Play("animations/vause_idle.csv");
	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
		animator.Play("animations/longvause_idle.csv");
	if (InputMgr::GetKeyDown(sf::Keyboard::Num3))
		animator.Play("animations/vause_formchange.csv"); 
	if (InputMgr::GetKeyDown(sf::Keyboard::Num4))
		animator.Play("animations/vause_attackformidle.csv");
	if (InputMgr::GetKeyDown(sf::Keyboard::Num5))
		animator.Play("animations/vause_die.csv");
	

	animator.Update(dt);
}

void Vause::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
