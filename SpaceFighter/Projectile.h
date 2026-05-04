
#pragma once

#include "KatanaEngine.h"
#include "GameObject.h"

/** @brief Represents a projectile that can be fired by a weapon. */
class Projectile : public GameObject
{

public:

	/** @brief Instantiate a projectile object. */
	Projectile();
	virtual ~Projectile() { }

	/** @brief Set the texture that will be used to render the projectile.
		@param pTexture A pointer to the texture. */
	static void SetTexture(Texture *pTexture) { s_pTexture = pTexture; }

	/** @brief Update the projectile.
		@param pGameTime Timing values including time since last update. */
	virtual void Update(const GameTime& gameTime);

	/** @brief Render the projectile.
		@param spriteBatch A reference to the game's sprite batch, used for rendering. */
	virtual void Draw(SpriteBatch& spriteBatch);

	/** @brief Activate the projectile.
		@param position The position to activate the projectile at.
		@param wasShotByPlayer A flag to determine if the projectile was fired by the player. */
	virtual void Activate(const Vector2 &position, bool wasShotByPlayer = true);

	/** @brief Get the amount of damage the projectile will deal.
		@return The amount of damage the projectile will deal. */
	virtual float GetDamage() const { return m_damage; }

	/** @brief Get the string representation of the projectile.
		@return The string representation of the projectile. */
	virtual std::string ToString() const;

	/** @brief Get the type of collision the projectile will have.
		@return The type of collision the projectile will have. */
	virtual CollisionType GetCollisionType() const;
	/** @brief Set the speed of the projectile.
		@param speed The speed of the projectile. */
	virtual void SetSpeed(const float speed) { m_speed = speed; }
	/** @brief Set the direction of the projectile.
		@param direction The direction of the projectile. */
	virtual void SetDirection(const Vector2 direction) { m_direction = direction; }


protected:

	

	/** @brief Set the amount of damage the projectile will deal.
		@param damage The amount of damage the projectile will deal. */
	virtual void SetDamage(const float damage) { m_damage = damage; }

	

	/** @brief Get the speed of the projectile.
		@return The speed of the projectile. */
	virtual float GetSpeed() const { return m_speed; }

	/** @brief Get the direction of the projectile.
		@return The direction of the projectile. */
	virtual Vector2 &GetDirection() { return m_direction; }

	/** @brief Determine if the projectile was fired by the player.
		@return True if the projectile was fired by the player. */
	virtual bool WasShotByPlayer() const { return m_wasShotByPlayer; }

	/** @brief Get the collision type of the projectile.
		@return The collision type of the projectile. */
	virtual CollisionType GetProjectileType() const { return CollisionType::Projectile; }


private:

	static Texture *s_pTexture;

	float m_speed = 500; // pixels per second
	float m_damage = 1;

	Vector2 m_direction;

	bool m_wasShotByPlayer = true;
};

