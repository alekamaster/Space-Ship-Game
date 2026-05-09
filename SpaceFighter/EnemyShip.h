#pragma once
#include "Ship.h"

enum class EnemyType { Normal, Burst, Speed, Laser, Multishot, Health };

class Projectile;
class LaserBeam;

class EnemyShip : public Ship
{
public:
	EnemyShip();
	virtual ~EnemyShip() {}

	virtual void SetType(EnemyType type);
	virtual void Update(const KatanaEngine::GameTime& gameTime) override;
	virtual void Draw(KatanaEngine::SpriteBatch& spriteBatch) override;
	virtual void Initialize(const KatanaEngine::Vector2 position, const double delaySeconds);
	virtual KatanaEngine::Vector2 GetHalfDimensions() const override;
	virtual void Hit(const float damage) override;

	// Implement the pure virtual from Ship to make EnemyShip concrete
	virtual CollisionType GetCollisionType() const override { return CollisionType::Enemy | CollisionType::Ship; }

	void SetTexture(KatanaEngine::Texture* pTexture) { m_pTexture = pTexture; }
	void SetProjectilePool(std::vector<Projectile*>* pProjectiles) { m_pProjectiles = pProjectiles; }
	void SetLaserPool(std::vector<LaserBeam*>* pLaserBeams) { m_pLaserBeams = pLaserBeams; }

	// Método para que el Level (y el futuro HUD) sepa cuántos puntos dar
	int GetScoreValue() const { return m_scoreValue; }

private:
	void Fire();

	KatanaEngine::Texture* m_pTexture = nullptr;
	EnemyType m_type = EnemyType::Normal;

	double m_delaySeconds = 0;
	double m_activationSeconds = 0;

	std::vector<Projectile*>* m_pProjectiles = nullptr;
	std::vector<LaserBeam*>* m_pLaserBeams = nullptr;

	float m_shootTimer = 0.0f;
	float m_shootCooldown = 2.0f;

	
	// --- CONTROL DE RÁFAGAS ---
	int m_burstShotsFired = 3;
	float m_scale = 1.0f;        // Control de tamaño físico y visual
	int m_scoreValue = 10;       // Recompensa de puntos
	float m_flashTimer = 0.0f;   // Temporizador para el parpadeo rojo de daño
};