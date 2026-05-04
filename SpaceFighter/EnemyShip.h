#pragma once

#include "Ship.h"
#include <vector>

class Projectile;

class LaserBeam;

enum class EnemyType
{
	Normal,
	Burst,
	Speed,
	Laser,
	Multishot,
	Health
};

/** @brief Represents an enemy ship. */
class EnemyShip : public Ship
{

public:

	//laser
	virtual void SetLaserPool(std::vector<LaserBeam*>* pLasers) { m_pLaserBeams = pLasers; }
	virtual void SetTexture(Texture* pTexture) { m_pTexture = pTexture; }
	//virtual void SetTexture(KatanaEngine::Texture* pTexture) { m_pTexture = pTexture; }
	/** @brief Creates a new instance of EnemyShip. */
	EnemyShip();
	virtual ~EnemyShip() { }

	/** @brief Loads the content for the enemy ship.
		@param resourceManager A reference to the game's resource manager,
		used for loading and managing game assets (resources). */
	virtual void Update(const GameTime& gameTime);

	/** @brief Draws the enemy ship.
		@param spriteBatch A reference to the game's sprite batch, used for rendering. */
	virtual void Draw(KatanaEngine::SpriteBatch& spriteBatch) override
	{
		if (IsActive() && m_pTexture)
		{
			spriteBatch.Draw(
				m_pTexture,
				GetPosition(),
				KatanaEngine::Color::WHITE,
				m_pTexture->GetCenter(),
				KatanaEngine::Vector2(m_scale, m_scale),
				
				0.0f
			);
		}
	}

	/** @brief Initializes the enemy ship.
		@param position The starting position of the enemy ship.
		@param delaySeconds The delay before the enemy ship activates. */
	virtual void Initialize(const Vector2 position, const double delaySeconds);

	// Set the type of the enemy ship.

	virtual void SetType(EnemyType type);
	virtual EnemyType GetType() const { return m_type; }

	//conexion with the level
	virtual void SetProjectilePool(std::vector<Projectile*>* pProjectiles) { m_pProjectiles = pProjectiles; }



	/** @brief Fires a weapon from the enemy ship. */
	virtual void Fire();

	/** @brief Applies damage to the ship.
		@param damage The amount of damage to apply. */
	virtual void Hit(const float damage);

	/** @brief Gets the string representation of the enemy ship.
		@return Returns the string "Enemy Ship". */
	virtual std::string ToString() const override{ return "Enemy Ship"; }

	/** @brief Gets the collision type of the enemy ship.
		@return Returns the collision type of the enemy ship. */
	virtual CollisionType GetCollisionType() const { return CollisionType::Enemy | CollisionType::Ship; }


protected:

	/** @brief Gets the delay before the enemy ship activates,
		to prevent all enemy ships from activating at the same time.
		@return Returns the delay before the enemy ship activates. */
	virtual double GetDelaySeconds() const { return m_delaySeconds; }
	KatanaEngine::Texture* m_pTexture = nullptr;


private:
    
	double m_delaySeconds = 0;
	double m_activationSeconds = 0;
	
	//Enemy shoot logic
	EnemyType m_type = EnemyType::Normal;
	std::vector<Projectile*>* m_pProjectiles = nullptr;

	float m_shootTimer = 0.0f;
	float m_shootCooldown = 2.0f; // Time in seconds between shots

	std::vector<LaserBeam*>* m_pLaserBeams = nullptr;
	float m_scale = 1.0f;
};
