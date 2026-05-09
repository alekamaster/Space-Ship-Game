#include "EnemyShip.h"
#include "Level.h"
#include "Projectile.h"
#include "LaserBeam.h"
#include "PlayerShip.h" 
#include <cstdlib>      

EnemyShip::EnemyShip()
{
	SetMaxHitPoints(1);
	SetCollisionRadius(20);
	m_shootTimer = (float)(rand() % 200) / 100.0f;
}

void EnemyShip::SetType(EnemyType type)
{
	m_type = type;

	// Configuración de Estadísticas, Escalas y Puntos
	switch (m_type)
	{
	case EnemyType::Burst:
		SetMaxHitPoints(2);
		m_shootCooldown = 0.5f;
		SetSpeed(50);
		m_scale = 0.3f;
		m_scoreValue = 20;
		break;
	case EnemyType::Speed:
		SetMaxHitPoints(1);
		m_shootCooldown = 999.0f; // Kamikaze
		SetSpeed(200);
		m_scale = 0.4f; // Muy pequeño
		m_scoreValue = 15;
		break;
	case EnemyType::Health:
		SetMaxHitPoints(10);
		m_shootCooldown = 2.0f;
		SetSpeed(20);
		m_scale = 0.4f; // Gigante
		m_scoreValue = 100; // Gran recompensa
		break;
	case EnemyType::Laser:
		SetMaxHitPoints(1);
		m_shootCooldown = 4.0f;
		SetSpeed(30);
		m_scale = 0.3f;
		m_scoreValue = 50;
		break;
	case EnemyType::Multishot:
		SetMaxHitPoints(1);
		m_shootCooldown = 1.5f;
		SetSpeed(40);
		m_scale = 0.3f;
		m_scoreValue = 30;
		break;
	default: // Normal
		SetMaxHitPoints(1);
		m_shootCooldown = 2.0f;
		SetSpeed(50);
		m_scale = 0.6f;
		m_scoreValue = 10;
		break;
	}
}

void EnemyShip::Update(const GameTime& gameTime)
{
	if (m_delaySeconds > 0)
	{
		m_delaySeconds -= gameTime.GetElapsedTime();
		if (m_delaySeconds <= 0) GameObject::Activate();
	}

	if (IsActive())
	{
		m_activationSeconds += gameTime.GetElapsedTime();
		if (m_activationSeconds > 2 && !IsOnScreen()) Deactivate();

		if (m_flashTimer > 0.0f) m_flashTimer -= (float)gameTime.GetElapsedTime();

		if (m_type == EnemyType::Speed)
		{
			PlayerShip* pPlayer = GetCurrentLevel()->GetClosestObject<PlayerShip>(GetPosition(), 0);
			if (pPlayer)
			{
				Vector2 direction = pPlayer->GetPosition() - GetPosition();
				direction.Normalize();
				TranslatePosition(direction * GetSpeed() * gameTime.GetElapsedTime());
			}
		}
		else
		{
			if (GetPosition().Y < 100)
			{
				TranslatePosition(Vector2::UNIT_Y * (GetSpeed() * 2) * gameTime.GetElapsedTime());
			}
			else
			{
				TranslatePosition(Vector2::UNIT_Y * (GetSpeed() * 0.2f) * gameTime.GetElapsedTime());
			}
		}

		// --- LÓGICA DE DISPARO BLINDADA ---
		if (m_type != EnemyType::Speed)
		{
			m_shootTimer -= (float)gameTime.GetElapsedTime();
			if (m_shootTimer <= 0.0f)
			{
				Fire(); // Dispara primero

				if (m_type == EnemyType::Burst)
				{
					m_burstShotsFired++;
					if (m_burstShotsFired >= 3)
					{
						m_shootTimer = 2.0f; // Pausa larga
						m_burstShotsFired = 0; // Reset
					}
					else
					{
						m_shootTimer = m_shootCooldown; // Pausa corta
					}
				}
				else
				{
					m_shootTimer = m_shootCooldown; // Nave normal
				}
			}
		}
	}

	Ship::Update(gameTime);
}

void EnemyShip::Initialize(const Vector2 position, const double delaySeconds)
{
	SetPosition(position);
	m_delaySeconds = delaySeconds;

	if (m_pTexture != nullptr)
	{
		SetCollisionRadius(m_pTexture->GetCenter().X * m_scale);
	}

	// --- EL LAVADO DE CEREBRO (RESET) ---
	m_flashTimer = 0.0f;
	m_burstShotsFired = 0;
	m_shootTimer = 1.0f; // Les damos 1 segundo de paz al aparecer antes de que disparen

	Ship::Initialize();
}

void EnemyShip::Draw(SpriteBatch& spriteBatch)
{
	if (IsActive() && m_pTexture != nullptr)
	{
		const float alpha = GetCurrentLevel()->GetAlpha();
		Color drawColor = (m_flashTimer > 0.0f) ? Color::RED : Color::WHITE;

		spriteBatch.Draw(
			m_pTexture,
			GetPosition(),
			drawColor * alpha,
			m_pTexture->GetCenter(),
			Vector2(m_scale, m_scale),
			0.0f
		);
	}
}

Vector2 EnemyShip::GetHalfDimensions() const
{
	if (m_pTexture != nullptr) return m_pTexture->GetCenter() * m_scale;
	return Vector2::ZERO;
}

void EnemyShip::Hit(const float damage)
{
	m_flashTimer = 0.1f;
	Ship::Hit(damage);
}

void EnemyShip::Fire()
{
	if (!m_pProjectiles) return;
	const float bulletSpeed = 300.0f;

	switch (m_type)
	{
	case EnemyType::Normal:
	case EnemyType::Burst:
	case EnemyType::Health:
	{
		Projectile* pBullet = nullptr;
		for (Projectile* p : *m_pProjectiles)
		{
			if (!p->IsActive()) { pBullet = p; break; }
		}
		if (pBullet)
		{
			pBullet->Activate(GetPosition(), false);
			pBullet->SetDirection(Vector2::UNIT_Y);
			pBullet->SetSpeed(bulletSpeed);
		}
		break;
	}
	case EnemyType::Multishot:
	{
		Vector2 directions[3] = { Vector2(0, 1), Vector2(-0.3f, 1), Vector2(0.3f, 1) };
		for (int i = 0; i < 3; i++)
		{
			Projectile* pBullet = nullptr;
			for (Projectile* p : *m_pProjectiles)
			{
				if (!p->IsActive()) { pBullet = p; break; }
			}
			if (pBullet)
			{
				directions[i].Normalize();
				pBullet->Activate(GetPosition(), false);
				pBullet->SetDirection(directions[i]);
				pBullet->SetSpeed(bulletSpeed);
			}
		}
		break;
	}
	case EnemyType::Laser:
	{
		if (!m_pLaserBeams) break;
		LaserBeam* pLaser = nullptr;
		for (LaserBeam* l : *m_pLaserBeams)
		{
			if (!l->IsActive()) { pLaser = l; break; }
		}
		if (pLaser)
		{
			Vector2 shipBottom = GetPosition() + Vector2(0.0f, GetHalfDimensions().Y);
			float laserHalfLength = pLaser->GetHalfDimensions().Y;
			Vector2 exactSpawnPosition = shipBottom + Vector2(0.0f, laserHalfLength);
			pLaser->Activate(exactSpawnPosition, 1.0f);
		}
		break;
	}
	}
}