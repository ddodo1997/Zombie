#pragma once
class SceneGame;
class ItemMaker;
class Player : public GameObject
{
public:
	enum class Weapon {
		Pistol,
		Assault,
		ShotGun,
	};

protected:
	sf::Sprite body;
	std::string texPlayerId = "graphics/player.png";

	Weapon currentWeapon = Weapon::Pistol;
	Weapon prevWeapon = currentWeapon;

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

	//TODO : 업그레이드시 올라갈 능력치 변수 따로 만들어서 계산하기.

	float upgradeFireRate = 0.f;
	int upgradeClipSize = 0;
	int upgradeMaxHp = 0;
	float upgradeRunSpeed = 0.f;
	int upgradeHealing = 0;
	int upgradePickAmmo = 0;

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
	//Pistol
	//Assault
	//ShotGun

	void SetWeapon(Weapon weapon);

	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	bool Shoot();
	bool ShootPistol();
	bool ShootAssault();
	bool ShootShotGun();

	void Reloading();

	void OnDamege(int damage);

	int GetAmmo() const { return ammo; }
	int GetSpareAmmo() const { return spareAmmo; }
	int GetHp() const { return hp; }
	int GetMaxHp() const { return maxHp; }

	void Awake();

	void UpgradeFireRate(float fireRate) { upgradeFireRate += fireRate; }
	void UpgradeClipSize(int size) { upgradeClipSize += size; }
	void UpgradeMaxHp(int maxHp) { upgradeMaxHp += maxHp; }
	void UpgradeSpeed(float speed) { upgradeRunSpeed += speed; }
	void UpgradeBullets(int bullets) { upgradePickAmmo += bullets; }
	void UpgradeHealing(int heal) { upgradeHealing += heal; }
};


