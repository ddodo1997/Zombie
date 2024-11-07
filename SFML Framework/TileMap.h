#pragma once

class TileMap : public GameObject
{
protected:
	sf::VertexArray va;
	sf::Texture* texture;
	std::string texTileMapId = "graphics/background_sheet.png";

	sf::Vector2i cellcount;
	sf::Vector2f cellsize;

	sf::Transform transform;
public:
	TileMap(const std::string& name = "");
	~TileMap() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::FloatRect GetLocalBounds()const override;
	sf::FloatRect GetGlobalBounds()const override;
	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void Set(sf::Vector2i count, sf::Vector2f size);
	sf::Vector2i GetCellCount() const { return cellcount; }
	sf::Vector2f GetCellSize() const { return cellsize; }
	void UpdateTransform();
};
