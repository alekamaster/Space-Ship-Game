#pragma once
#include "EnemyShip.h"

/** @brief Represents an enemy ship that is biological in nature. */
class BioEnemyShip : public EnemyShip
{
public:

	/** @brief Creates a new instance of BioEnemyShip. */
	BioEnemyShip();
	virtual ~BioEnemyShip();

	// Sobrescribimos los métodos virtuales que podrían quedar como abstractos
	virtual void SetType(EnemyType type) override;
	virtual void Update(const KatanaEngine::GameTime& gameTime) override;
	virtual void Draw(KatanaEngine::SpriteBatch& spriteBatch) override;
	virtual void Initialize(const KatanaEngine::Vector2 position, const double delaySeconds) override;
	virtual KatanaEngine::Vector2 GetHalfDimensions() const override;
	virtual void Hit(const float damage) override;

	// Ya no necesitamos reescribir SetTexture u otros si no son puramente virtuales.
};