#pragma once

class UiHud : public GameObject
{
protected:
	//상단
	sf::Text textScore;
	sf::Text textHighScore;
	sf::Text textUpgrade;
	//중앙
	sf::Text textCenter;
	//하단
	sf::Sprite iconAmmo;
	sf::Text textAmmo;
	sf::RectangleShape gaugeHp;
	sf::Text textInterval;
	sf::Text textWave;
	sf::Text textZombieCount;

	//체력 게이지 크기
	sf::Vector2f gaugeHpMaxSize = { 400.f, 50.f };

	
public:
	UiHud(const std::string& name = "");
	~UiHud() = default;

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

	void SetScore(int s);
	void SetUpgrade(int u);
	void SetHiScore(int s);
	void SetCenter(std::wstring str);
	void SetInterval(int i);
	void SetAmmo(int current, int total);
	void SetHp(int hp, int max);
	void SetWave(int w);
	void SetZombieCount(int count);
};
