#pragma once

class AniPlayer : public GameObject
{
protected:
	sf::Sprite body;
	Animator animator;

	float speed = 1000.f;
	sf::Vector2f gravity = { 0.f,1000.f };
	sf::Vector2f velocity = { 0.f,0.f };
	bool isGround = true;


public:
	AniPlayer(const std::string& name = "");
	~AniPlayer() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	
	void Draw(sf::RenderWindow& window) override;
};
