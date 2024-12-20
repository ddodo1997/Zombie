#pragma once

class SceneGame;
class UiUpgrade : public GameObject
{
public:
	enum class Upgrades {
		FireRate,
		ClipSize,
		MaxHp,
		RunSpeed,
		MoreHealth,
		MoreAmmo,
	};
	static const int TotalUpgrades = 6;
protected:
	sf::Sprite backGround;
	std::vector<sf::Text> textUpgrades;

	SceneGame* sceneGame = nullptr;
public:
	UiUpgrade(const std::string& name = "");
	~UiUpgrade() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt)override;
	void Draw(sf::RenderWindow& window) override;
	void OnLocallize(Languages lang)override;

	void SetText(sf::Text& text, Upgrades upgrade);
};