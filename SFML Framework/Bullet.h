#pragma once
class SceneGame;
class Bullet : public GameObject
{
protected:
	sf::Sprite body;
	std::string texBulletId = "graphics/bullet.png";

	sf::Vector2f direction;
	float speed = 0.f;
	int damage = 0;

	float activeDelay = 5.f;
	float activeTimer = 0.f;

	SceneGame* sceneGame;
public:
	Bullet(const std::string& name = "");
	~Bullet() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::FloatRect GetLocalBounds() const override { return body.getLocalBounds(); }
	sf::FloatRect GetGlobalBounds() const override { return body.getGlobalBounds(); }

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void Fire(const sf::Vector2f& pos, const sf::Vector2f& direction, float speed, int damage);

};
