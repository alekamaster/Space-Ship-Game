#include "EnemyShip.h"
#include "Level.h"
#include "Projectile.h"
#include "LaserBeam.h"
#include "PlayerShip.h" // <-- ¡LA PIEZA FALTANTE AÑADIDA AQUÍ!
#include <cstdlib>      // Necesario para la función rand()

EnemyShip::EnemyShip()
{
	SetMaxHitPoints(1);
	SetCollisionRadius(20);

	m_shootTimer = (float)(rand() % 200) / 100.0f;
}

void EnemyShip::SetType(EnemyType type)
{
	m_type = type;

	// Configuration based on the type of enemy ship
	switch (m_type)
	{
	case EnemyType::Burst:
		SetMaxHitPoints(2);
		m_shootCooldown = 0.5f; // Shoots rapidly
		SetSpeed(50);
		break;
	case EnemyType::Speed:
		SetMaxHitPoints(1);
		m_shootCooldown = 999.0f; // Does not shoot, it's kamikaze
		SetSpeed(200); // Very fast
		break;
	case EnemyType::Health:
		SetMaxHitPoints(10); // High health
		m_shootCooldown = 2.0f;
		SetSpeed(20); // Very slow
		break;
	case EnemyType::Laser:
		SetMaxHitPoints(1);
		m_shootCooldown = 4.0f; // reload time for the laser
		SetSpeed(30);
		break;
	case EnemyType::Multishot:
		SetMaxHitPoints(1);
		m_shootCooldown = 1.5f;
		SetSpeed(40);
		break;
	default: // Normal
		SetMaxHitPoints(1);
		m_shootCooldown = 2.0f;
		SetSpeed(50);
		break;
	}
}

void EnemyShip::Update(const KatanaEngine::GameTime& gameTime)
{
	// spawn delay logic
	if (m_delaySeconds > 0)
	{
		m_delaySeconds -= gameTime.GetElapsedTime();

		if (m_delaySeconds <= 0)
		{
			GameObject::Activate();
		}
	}

	// active ship logic
	if (IsActive())
	{
		m_activationSeconds += gameTime.GetElapsedTime();
		if (m_activationSeconds > 2 && !IsOnScreen()) Deactivate();

		// movement logic based on the type of enemy ship
		if (m_type == EnemyType::Speed)
		{
			// Kamikaze logic: Seek the player to collide
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
			// join the screen slowly from the top, and then stop
			if (GetPosition().Y < 100) // 100 pixels from the top of the screen
			{
				TranslatePosition(Vector2::UNIT_Y * (GetSpeed() * 2) * gameTime.GetElapsedTime());
			}
			else
			{
				// ships that are not kamikaze will move slowly downwards
				TranslatePosition(Vector2::UNIT_Y * (GetSpeed() * 0.2f) * gameTime.GetElapsedTime());
			}
		}

		// --- NEW SHOOTING LOGIC ---
		// Validate that it is not the Kamikaze ship, as it should not shoot
		if (m_type != EnemyType::Speed)
		{
			m_shootTimer -= gameTime.GetElapsedTime();
			if (m_shootTimer <= 0.0f)
			{
				Fire();
				m_shootTimer = m_shootCooldown; // time reset
			}
		}
	}

	Ship::Update(gameTime);
}

void EnemyShip::Initialize(const Vector2 position, const double delaySeconds)
{
	SetPosition(position);
	m_delaySeconds = delaySeconds;

	Ship::Initialize();
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
		// Shoot a single bullet straight down
		Projectile* pBullet = nullptr;

		// Find an inactive projectile in the pool
		for (Projectile* p : *m_pProjectiles)
		{
			if (!p->IsActive())
			{
				pBullet = p;
				break;
			}
		}

		if (pBullet)
		{
			// Activate the bullet (position, wasShotByPlayer = false)
			pBullet->Activate(GetPosition(), false);

			// Set the downward direction and speed
			pBullet->SetDirection(Vector2::UNIT_Y);
			pBullet->SetSpeed(bulletSpeed);
		}
		break;
	}
	case EnemyType::Multishot:
	{
		// Shoot three bullets in a spread pattern
		Vector2 directions[3] = {
			Vector2(0, 1),       // Center (Straight down)
			Vector2(-0.3f, 1),   // Left Diagonal
			Vector2(0.3f, 1)     // Right Diagonal
		};

		for (int i = 0; i < 3; i++)
		{
			Projectile* pBullet = nullptr;

			// Find an inactive projectile in the pool for each direction
			for (Projectile* p : *m_pProjectiles)
			{
				if (!p->IsActive())
				{
					pBullet = p;
					break;
				}
			}

			if (pBullet)
			{
				directions[i].Normalize();

				// Activate and set specific direction and speed
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

		// Find an inactive laser beam in the pool
		for (LaserBeam* l : *m_pLaserBeams)
		{
			if (!l->IsActive())
			{
				pLaser = l;
				break;
			}
		}

		if (pLaser)
		{
			// Activate the laser for 1.0 second
			pLaser->Activate(GetPosition(), 1.0f);
		}
		break;
	}
	}
}

void EnemyShip::Hit(const float damage)
{
	Ship::Hit(damage);
}