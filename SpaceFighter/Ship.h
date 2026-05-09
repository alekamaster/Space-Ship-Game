
#pragma once

#include "GameObject.h"
#include "Weapon.h"

/** @brief Represents a ship in the game. */
class Ship : public GameObject, public IAttachable
{

public:

	/** @brief Creates a new instance of Ship. */
	Ship();
	virtual ~Ship() { }
	/** @brief Gets the max hit points of the ship.
		@return Returns the max hit points of the ship. */
	virtual float GetMaxHitPoints() const { return m_maxHitPoints; }
	/** @brief Updates the ship.
		@param pGameTime A pointer to the game time object. */
	virtual void Update(const GameTime& gameTime);

	/** @brief Draws the ship.
		@param spriteBatch A reference to the game's sprite batch, used for rendering. */
	virtual void Draw(SpriteBatch& spriteBatch) = 0;

	/** @brief Applies damage to the ship.
		@param damage The amount of damage to apply. */
	virtual void Hit(const float damage);

	/** @brief To determine if the ship should take damage when hit.
		@return True if the ship is invulnerable, false otherwise. */
	virtual bool IsInvulnurable() const { return m_isInvulnurable; }

	/** @brief Sets the invulnerability of the ship.
		@param isInvulnurable True to make the ship invulnerable, false otherwise. */
	virtual void SetInvulnurable(bool isInvulnurable = true) { m_isInvulnurable = isInvulnurable; }

	/** @brief Gets the string representation of the ship.
		@return Returns the string "Ship". */
	virtual std::string ToString() const { return "Ship"; }

	/** @brief Gets the collision type of the ship.
		@return Returns the collision type of the ship. */
	virtual CollisionType GetCollisionType() const = 0;

	/** @brief Attaches a weapon to the ship.
		@param pWeapon A pointer to the weapon to attach.
		@param position The position on the ship that the weapon will be attached,
		offset from the center of the ship. */
	//virtual void AttachWeapon(Weapon *pWeapon, Vector2 position);

	virtual void AttachItem(IAttachment *item, Vector2 position);

	/** @brief Gets the speed of the ship.
		@return Returns the speed of the ship. */
	virtual float GetSpeed() const { return m_speed; }

	/** @brief Sets the speed of the ship.
		@param speed The speed of the ship. */
	virtual void SetSpeed(const float speed) { m_speed = speed; }

	/** @brief Sets the max hit points of the ship.
		@param hitPoints The max hit points of the ship. */
	virtual void SetMaxHitPoints(const float hitPoints) { m_maxHitPoints = hitPoints; }

	/** @brief Set the desired movement direction for the ship.
		@remark Default implementation is a no-op; derived classes (PlayerShip, EnemyShip) may override. */
	virtual void SetDesiredDirection(const Vector2& direction) { /* default: do nothing */ }

	virtual IAttachment* GetAttachment(const std::string& key);

	virtual IAttachment* GetAttachment(const int index);


protected:

	/** @brief Initializes the ship. */
	virtual void Initialize();

	/** @brief Fires the ship's weapons.
		@param type The type of trigger that fired the weapons. */
	virtual void FireWeapons(TriggerType type = TriggerType::All);

	/** @brief Gets a weapon from the ship's weapon list.
		@param index The index of the weapon to get.
		@return Returns a pointer to the weapon. */
	virtual Weapon* GetWeapon(const std::string &key) //{ if (index < m_weapons.size()) return m_weapons[index]; return nullptr; }
	{
		if (m_attachments.find(key) == m_attachments.end()) return nullptr;
		return dynamic_cast<Weapon *>(m_attachments[key]);
	}

	/** @brief Gets the current hit points of the ship.
		@return Returns the current hit points of the ship. */
	virtual float GetHitPoints() const { return m_hitPoints; }




private:

	float m_speed = 300; // pixels per second

	float m_maxHitPoints = 3;
	float m_hitPoints = m_maxHitPoints;

	bool m_isInvulnurable = false;

	std::map<std::string, IAttachment *> m_attachments;
	std::map<std::string, IAttachment*>::iterator m_attachmentIt;
};

