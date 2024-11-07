#pragma once
class SceneGame;
class ItemMaker;
class Player : public GameObject
{
protected:
	sf::Sprite body;
	std::string texPlayerId = "graphics/player.png";

	sf::Vector2f direction;
	float speed = 500.f;
	sf::Vector2f look;
	sf::FloatRect movalbeArea;

	int maxHp = 50;
	int hp = 0;
	int healHp = 10;
	int spareAmmo = 10;
	int maxAmmo = 6;
	int ammo = 0;
	int pickAmmo = 6;
	bool isAlive = true;
	bool needReloading = false;

	float shootDelay = 0.5f;
	float shootTimer = 0.f;

	float reloadDelay = 2.f;
	float reloadTimer = 0.f;
	bool isReloading = false;
	sf::RectangleShape reloadingBar;
	sf::Vector2f reloadingBarSize = { 75.f, 15.f };

	SceneGame* sceneGame;
	ItemMaker* itemMaker;
public:
	Player(const std::string& name = "");
	~Player() = default;

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

	bool Shoot();
	void Reloading();

	void OnDamege(int damage);

	int GetAmmo() const { return ammo; }
	int GetSpareAmmo() const { return spareAmmo; }
	int GetHp() const { return hp; }
	int GetMaxHp() const { return maxHp; }


	void UpgradeFireRate(float fireRate) { shootDelay -= shootDelay / fireRate; }
	void UpgradeClipSize(int size) { maxAmmo += size; }
	void UpgradeMaxHp(int maxHp) { this->maxHp += maxHp; }
	void UpgradeSpeed(float speed) { this->speed += speed; }
	void UpgradeBullets(int bullets) { pickAmmo += bullets; }
	void UpgradeHealing(int heal) { healHp += heal; }
};


