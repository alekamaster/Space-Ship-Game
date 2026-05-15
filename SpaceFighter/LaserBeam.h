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
	virtual float GetCollisionRadius() const override
	{
		if (m_pTexture != nullptr) return m_pTexture->GetCenter().Y * m_lengthScale;
		return 0.5f;
	}

	// --- NUEVO: Método para alinear las físicas con el escalado visual ---
	// Declaración solamente: la implementación se encuentra en LaserBeam.cpp
	virtual Vector2 GetHalfDimensions() const override;

	/** @brief Gets the collision type mask for the laser beam. */
	virtual CollisionType GetCollisionType() const override
	{
		// Laser beams are enemy projectiles for collision purposes
		return CollisionType::Enemy | CollisionType::Projectile;
	}

	/** @brief Check if laser has already dealt damage this activation. */
	bool HasDealtDamage() const { return m_hasDealtDamage; }

	/** @brief Mark that this laser has dealt damage. */
	void MarkDamageDealt() { m_hasDealtDamage = true; }

private:
	float m_lifetime = 0.0f;
	Texture* m_pTexture = nullptr;
	bool m_hasDealtDamage = false; // Track if this laser has already hit something

	// --- CONTROL DE ESCALA SEPARADO ---
	float m_widthScale = 0.3f;  // X: Controla qué tan grueso es el láser
	float m_lengthScale = 2.0f; // Y: Controla el largo (2.0f lo hace el doble de largo)
};
