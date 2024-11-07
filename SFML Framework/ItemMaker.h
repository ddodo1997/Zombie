#pragma once
class Item;
class ItemMaker : public GameObject
{
protected:
	sf::FloatRect area;

	std::list<Item*> activeItems;
	ObjectPool<Item> itemPool;
public:
	ItemMaker(const std::string& name = "");
	~ItemMaker() = default;

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

	void MakeItem(const sf::FloatRect& area, int count);

	const std::list<Item*>& GetActiveItems() { return activeItems; }
	void OnCatchItem(Item* item);
};