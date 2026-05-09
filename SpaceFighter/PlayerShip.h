#pragma once

#include "KatanaEngine.h"
#include "Ship.h"
#include <string>

using namespace KatanaEngine;

class Level;

/** @brief Represents the player's ship in the game. */
class PlayerShip : public Ship
{
public:
    PlayerShip() {}
    virtual ~PlayerShip() {}

    // --- GLOBAL CUSTOMIZATION VARIABLES ---
    static std::string SelectedTexturePath;
    static float SelectedScale;
    static float SelectedSpeed;
    static int SelectedMaxHitPoints;

    /** @brief Configures and saves the selected ship along with its stats.
        @param shipIndex The index of the selected ship from the menu. */
    static void SetSelectedShip(int shipIndex);

    // --- BASE METHODS ---
    virtual void LoadContent(ResourceManager& resourceManager);
    virtual void Initialize(Level* pLevel, Vector2& startPosition);
    virtual void HandleInput(const InputState& input);
    virtual void Update(const GameTime& gameTime);
    virtual void Draw(SpriteBatch& spriteBatch);
    virtual std::string ToString() const override { return "Player Ship"; }
    virtual CollisionType GetCollisionType() const override { return CollisionType::Player | CollisionType::Ship;; }
    /** @brief Gets the collision dimensions adjusted by the current scale. */
    virtual Vector2 GetHalfDimensions() const;

    void SetResponsiveness(const float responsiveness);
    float GetResponsiveness() const { return m_responsiveness; }
    void SetDesiredDirection(const Vector2& direction) { m_desiredDirection = direction; }

protected:
    void ConfineToScreen() { m_isConfinedToScreen = true; }

private:
    float m_responsiveness = 0.1f;
    Vector2 m_desiredDirection = Vector2::ZERO;
    bool m_isConfinedToScreen = true;
    Texture* m_pTexture = nullptr;
    Vector2 m_velocity = Vector2::ZERO;
};