#pragma once
#include "GameObject.h"
#include "Texture.h"

/** @brief Represents a continuous laser beam fired by an enemy. */
class LaserBeam : public GameObject
{
public:
	virtual std::string ToString() const override { return "Laser Beam"; }
	virtual void Initialize() {}
	LaserBeam();
	virtual ~LaserBeam() {}
	
	/** @brief Activates the laser beam at a specific location for a set duration. */
	virtual void Activate(const Vector2 position, const float lifetime);

	virtual void Update(const GameTime& gameTime) override;

	virtual void Draw(SpriteBatch& spriteBatch) override;

	/** @brief Gets the damage dealt by the laser beam. */
	virtual float GetDamage() const { return 1.0f; }

	/** @brief Sets the texture for the laser beam. */
	virtual void SetTexture(Texture* pTexture) { m_pTexture = pTexture; }

	/** @brief Gets the collision type, masking it as an enemy projectile. */
	virtual CollisionType GetCollisionType() const override { return CollisionType::Enemy | CollisionType::Projectile; }

private:
	float m_lifetime = 0.0f;
	Texture* m_pTexture = nullptr;
};