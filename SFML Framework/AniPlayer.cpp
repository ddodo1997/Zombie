#include "stdafx.h"
#include "AniPlayer.h"

AniPlayer::AniPlayer(const std::string& name)
	: GameObject(name)
{
}

void AniPlayer::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void AniPlayer::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void AniPlayer::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void AniPlayer::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void AniPlayer::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void AniPlayer::Init()
{
	animator.SetTarget(&body);
}

void AniPlayer::Release()
{
}

void AniPlayer::Reset()
{
	animator.Play("animations/idle.csv");
	SetOrigin(Origins::BC);
}

void AniPlayer::Update(float dt)
{
	float h = 0.f;
	if (isGround)
	{
		h = InputMgr::GetAxis(Axis::Horizontal);
		velocity.x = h * speed;
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Space) && isGround)
	{
		isGround = false;
		velocity.y = -500.f;
		animator.Play("animations/jump.csv");
	}

	if (!isGround)
	{
		velocity += gravity * dt;
	}

	position += velocity * dt;

	if (position.y > 0.f)
	{
		velocity.y = 0.f;
		position.y = 0.f;
		isGround = true;
	}

	SetPosition(position);

	if (h != 0.f)
	{
		SetScale({ h > 0.f ? 1.f : -1.f ,1.f });
	}

	if (animator.GetCurrentClipId() == "Idle")
	{
		if (h != 0.f)
			animator.Play("animations/run.csv");
	}
	else if (animator.GetCurrentClipId() == "Run")
	{
		if (h == 0.f)
		{
			animator.Play("animations/idle.csv");
		}
	}
	else if (animator.GetCurrentClipId() == "Jump" && isGround)
	{
		if (h == 0.f)
		{
			animator.Play("animations/idle.csv");
		}
		else
		{
			animator.Play("animations/run.csv");
		}
	}

	animator.Update(dt);
}

void AniPlayer::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
