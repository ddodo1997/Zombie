#pragma once

class Vause : public GameObject
{
protected:
	sf::Sprite body;
	Animator animator;

	sf::Vector2f direction;
	float speed = 700.f;

public:
	Vause(const std::string& name = "");
	~Vause() = default;

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
