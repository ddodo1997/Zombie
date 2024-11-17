#pragma once
class Player;
class SceneGame;
class Zombie : public GameObject
{
public:
	enum class Types
	{
		Bloater,
		Chaser,
		Crawler,
		Nugget
	};
	static const int TotalTypes = 3;
protected:
	sf::Sprite body;
	std::string texZombieId;
	sf::Vector2f direction;
	sf::RectangleShape hpBar;
	sf::Vector2f hpBarSize = { 75.f, 15.f };
	Types types = Types::Bloater;
	Player* player;

	int maxHp = 0;
	int attack = 0;
	float speed = 0.f;
	float attackDelay = 0.f;

	int hp = 0;
	float attackTimer = 0.f;

	bool isNugget = false;
	float nuggetDelay = 3.f;
	float nuggetTimer = 0.f;

	SceneGame* sceneGame = nullptr;
public:
	Zombie(const std::string& name = "");
	~Zombie() = default;

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
	void FixedUpdate(float dt)override;
	void Draw(sf::RenderWindow& window) override;

	void SetType(Types types);

	void OnDamage(int damage);
	Types GetType() const { return types; }

	SaveZombie GetSaveData() const;
	void LoadSaveData(const SaveZombie& data);
};
