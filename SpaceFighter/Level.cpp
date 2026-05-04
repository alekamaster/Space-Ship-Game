#include "Level.h"
#include "EnemyShip.h"
#include "LaserBeam.h" // Needed to instantiate the continuous laser pool
#include "Blaster.h"
#include "GameplayScreen.h"

std::vector<Explosion*> Level::s_explosions;

// --- Collision Callback Functions ---

/** @brief Callback function for when the player shoots an enemy. */
void PlayerShootsEnemy(GameObject* pObject1, GameObject* pObject2)
{
	bool m = pObject1->HasMask(CollisionType::Enemy);
	EnemyShip* pEnemyShip = (EnemyShip*)((m) ? pObject1 : pObject2);
	Projectile* pPlayerProjectile = (Projectile*)((!m) ? pObject1 : pObject2);

	pEnemyShip->Hit(pPlayerProjectile->GetDamage());
	pPlayerProjectile->Deactivate();
}

/** @brief Callback function for when an enemy shoots the player. */
void EnemyShootsPlayer(GameObject* pObject1, GameObject* pObject2)
{
	bool m = pObject1->HasMask(CollisionType::Player);
	PlayerShip* pPlayerShip = (PlayerShip*)((m) ? pObject1 : pObject2);
	GameObject* pEnemyWeapon = (!m) ? pObject1 : pObject2;

	// Check if the enemy weapon is a LaserBeam
	LaserBeam* pLaser = dynamic_cast<LaserBeam*>(pEnemyWeapon);
	if (pLaser)
	{
		pPlayerShip->Hit(pLaser->GetDamage());
		// The laser DOES NOT deactivate! It stays alive until its timer ends.
	}
	else
	{
		// If it's not a laser, it's a normal projectile
		Projectile* pProjectile = dynamic_cast<Projectile*>(pEnemyWeapon);
		if (pProjectile)
		{
			pPlayerShip->Hit(pProjectile->GetDamage());
			pProjectile->Deactivate(); // Normal projectiles collide and disappear
		}
	}
}

/** @brief Callback function for when the player collides with an enemy. */
void PlayerCollidesWithEnemy(GameObject* pObject1, GameObject* pObject2)
{
	bool m = pObject1->HasMask(CollisionType::Player);
	PlayerShip* pPlayerShip = (PlayerShip*)((m) ? pObject1 : pObject2);
	EnemyShip* pEnemyShip = (EnemyShip*)((!m) ? pObject1 : pObject2);
	pPlayerShip->Hit(std::numeric_limits<float>::max());
	pEnemyShip->Hit(std::numeric_limits<float>::max());
}

Level::Level()
{
	m_sectorSize.X = 64;
	m_sectorSize.Y = 64;

	m_sectorCount.X = (float)((Game::GetScreenWidth() / (int)m_sectorSize.X) + 1);
	m_sectorCount.Y = (float)((Game::GetScreenHeight() / (int)m_sectorSize.Y) + 1);

	m_totalSectorCount = m_sectorCount.X * m_sectorCount.Y;

	m_pSectors = new std::vector<GameObject*>[m_totalSectorCount];
	m_pCollisionManager = new CollisionManager();

	GameObject::SetCurrentLevel(this);

	// Setup player ship
	m_pPlayerShip = new PlayerShip();
	Blaster* pBlaster = new Blaster("Main Blaster");
	pBlaster->SetProjectilePool(&m_projectiles);
	m_pPlayerShip->AttachItem(pBlaster, Vector2::UNIT_Y * -20);

	// Pool: Player Projectiles
	for (int i = 0; i < 100; i++)
	{
		Projectile* pProjectile = new Projectile();
		m_projectiles.push_back(pProjectile);
		AddGameObject(pProjectile);
	}

	m_pPlayerShip->Activate();
	AddGameObject(m_pPlayerShip);

	// Pool: Enemy Projectiles (FIXED: Properly spawn Projectiles, not EnemyShips)
	for (int i = 0; i < 100; i++)
	{
		Projectile* pProjectile = new Projectile();
		m_enemyProjectiles.push_back(pProjectile);
		AddGameObject(pProjectile);
	}

	// Pool: Enemy Laser Beams
	for (int i = 0; i < 20; i++)
	{
		LaserBeam* pLaser = new LaserBeam();
		m_enemyLasers.push_back(pLaser);
		AddGameObject(pLaser);
	}

	// Pool: Enemy Ships
	for (int i = 0; i < 20; i++) // 20 simultaneous ships on screen limit
	{
		EnemyShip* pEnemy = new EnemyShip();
		pEnemy->SetProjectilePool(&m_enemyProjectiles); // Assign standard projectiles
		pEnemy->SetLaserPool(&m_enemyLasers); // Assign laser beams
		m_enemies.push_back(pEnemy);
		AddGameObject(pEnemy);
	}

	// Setup collision types
	CollisionManager* pC = GetCollisionManager();

	CollisionType playerShip = (CollisionType::Player | CollisionType::Ship);
	CollisionType playerProjectile = (CollisionType::Player | CollisionType::Projectile);
	CollisionType enemyShip = (CollisionType::Enemy | CollisionType::Ship);
	CollisionType enemyProjectile = (CollisionType::Enemy | CollisionType::Projectile);

	pC->AddNonCollisionType(playerShip, playerProjectile);
	pC->AddNonCollisionType(enemyShip, enemyProjectile); // Enemies do not shoot each other

	pC->AddCollisionType(playerProjectile, enemyShip, PlayerShootsEnemy);
	pC->AddCollisionType(playerShip, enemyShip, PlayerCollidesWithEnemy);
	pC->AddCollisionType(playerShip, enemyProjectile, EnemyShootsPlayer);
}

Level::~Level()
{
	delete[] m_pSectors;
	delete m_pCollisionManager;

	m_gameObjectIt = m_gameObjects.begin();
	for (; m_gameObjectIt != m_gameObjects.end(); m_gameObjectIt++)
	{
		delete (*m_gameObjectIt);
	}
}

void Level::LoadContent(ResourceManager& resourceManager)
{
	m_pPlayerShip->LoadContent(resourceManager);

	m_pEnemyTextures[0] = resourceManager.Load<Texture>("Textures\\EnemyShipNormal.png");
	m_pEnemyTextures[1] = resourceManager.Load<Texture>("Textures\\EnemyShipBurst.png");
	m_pEnemyTextures[2] = resourceManager.Load<Texture>("Textures\\EnemyShipSpeed.png");
	m_pEnemyTextures[3] = resourceManager.Load<Texture>("Textures\\EnemyShipLaser.png");
	m_pEnemyTextures[4] = resourceManager.Load<Texture>("Textures\\EnemyShipMulti.png");
	m_pEnemyTextures[5] = resourceManager.Load<Texture>("Textures\\EnemyShipHealth.png");

	m_pLaserBeamTexture = resourceManager.Load<Texture>("Textures\\LaserBeam.png");

	for (LaserBeam* pLaser : m_enemyLasers)
	{
		pLaser->SetTexture(m_pLaserBeamTexture);
	}

	// Setup explosions if they haven't been already
	Explosion* pExplosion;
	if (s_explosions.size() == 0) {
		AudioSample* pExplosionSound = resourceManager.Load<AudioSample>("Audio\\Effects\\Explosion.ogg");
		Animation* pAnimation = resourceManager.Load<Animation>("Animations\\Explosion.anim");
		pAnimation->Stop();

		for (int i = 0; i < 10; i++)
		{
			pExplosion = new Explosion();
			pExplosion->SetAnimation((Animation*)pAnimation->Clone());
			pExplosion->SetSound(pExplosionSound);
			s_explosions.push_back(pExplosion);
		}
	}
}

void Level::HandleInput(const InputState& input)
{
	if (IsScreenTransitioning()) return;

	m_pPlayerShip->HandleInput(input);
}

void Level::Update(const GameTime& gameTime)
{
	// Enemy Spawn Manager 
	m_enemySpawnTimer -= (float)gameTime.GetElapsedTime();
	if (m_enemySpawnTimer <= 0.0f)
	{
		for (EnemyShip* pEnemy : m_enemies)
		{
			if (!pEnemy->IsActive())
			{
				// Choose a random type (from 0 to 5)
				int randomType = rand() % 6;
				pEnemy->SetType(static_cast<EnemyType>(randomType));

				// Assign the texture directly to the EnemyShip base class
				pEnemy->SetTexture(m_pEnemyTextures[randomType]);

				// Choose a random X position at the top edge
				int margin = 50;
				float randomX = margin + (float)(rand() % (Game::GetScreenWidth() - (margin * 2)));
				Vector2 spawnPosition(randomX, -50.0f);

				// Launch it into the scene!
				pEnemy->Initialize(spawnPosition, 0.0);

				break; // Only spawn one enemy per cycle
			}
		}

		// Reset the timer (e.g., 1.5 seconds)
		m_enemySpawnTimer = 1.5f;
	}

	for (unsigned int i = 0; i < m_totalSectorCount; i++)
	{
		m_pSectors[i].clear();
	}

	m_gameObjectIt = m_gameObjects.begin();
	for (; m_gameObjectIt != m_gameObjects.end(); m_gameObjectIt++)
	{
		GameObject* pGameObject = (*m_gameObjectIt);
		pGameObject->Update(gameTime);
	}

	for (unsigned int i = 0; i < m_totalSectorCount; i++)
	{
		if (m_pSectors[i].size() > 1)
		{
			CheckCollisions(m_pSectors[i]);
		}
	}

	for (Explosion* pExplosion : s_explosions) pExplosion->Update(gameTime);

	if (!m_pPlayerShip->IsActive()) GetGameplayScreen()->Exit();
}

void Level::UpdateSectorPosition(GameObject* pGameObject)
{
	Vector2 position = pGameObject->GetPosition();
	Vector2 halfDimensions = pGameObject->GetHalfDimensions();

	int minX = (int)(position.X - halfDimensions.X - 0.5f);
	int maxX = (int)(position.X + halfDimensions.X + 0.5f);
	int minY = (int)(position.Y - halfDimensions.Y - 0.5f);
	int maxY = (int)(position.Y + halfDimensions.Y + 0.5f);

	minX = Math::Clamp<int>(0, m_sectorCount.X - 1, minX / (int)m_sectorSize.X);
	maxX = Math::Clamp<int>(0, m_sectorCount.X - 1, maxX / (int)m_sectorSize.X);
	minY = Math::Clamp<int>(0, m_sectorCount.Y - 1, minY / (int)m_sectorSize.Y);
	maxY = Math::Clamp<int>(0, m_sectorCount.Y - 1, maxY / (int)m_sectorSize.Y);


	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			int index = y * (int)m_sectorCount.X + x;

			m_pSectors[index].push_back(pGameObject);
		}
	}
}

void Level::SpawnExplosion(GameObject* pExplodingObject)
{
	Explosion* pExplosion = nullptr;

	for (unsigned int i = 0; i < s_explosions.size(); i++)
	{
		if (!s_explosions[i]->IsActive())
		{
			pExplosion = s_explosions[i];
			break;
		}
	}

	if (!pExplosion) return;

	const float aproximateTextureRadius = 120;
	const float objectRadius = pExplodingObject->GetCollisionRadius();
	const float scaleToObjectSize = (1 / aproximateTextureRadius) * objectRadius * 2;
	const float dramaticEffect = 2.2f;
	const float scale = scaleToObjectSize * dramaticEffect;
	pExplosion->Activate(pExplodingObject->GetPosition(), scale);
}

float Level::GetAlpha() const
{
	return GetGameplayScreen()->GetAlpha();
}

void Level::CheckCollisions(std::vector<GameObject*>& gameObjects)
{
	const unsigned int objectCount = (unsigned int)gameObjects.size();

	GameObject* pFirst, * pSecond;

	for (unsigned int i = 0; i < objectCount - 1; i++)
	{
		pFirst = gameObjects[i];
		if (pFirst->IsActive())
		{
			for (unsigned int j = i + 1; j < objectCount; j++)
			{
				if (!pFirst->IsActive()) continue;

				pSecond = gameObjects[j];
				if (pSecond->IsActive())
				{
					m_pCollisionManager->CheckCollision(pFirst, pSecond);
				}
			}
		}
	}
}

void Level::Draw(SpriteBatch& spriteBatch)
{
	spriteBatch.Begin();

	const float alpha = GetGameplayScreen()->GetAlpha();

	if (m_pBackground) spriteBatch.Draw(m_pBackground, Vector2::ZERO, Color::WHITE * alpha);

	m_gameObjectIt = m_gameObjects.begin();
	for (; m_gameObjectIt != m_gameObjects.end(); m_gameObjectIt++)
	{
		GameObject* pGameObject = (*m_gameObjectIt);
		pGameObject->Draw(spriteBatch);
	}

	spriteBatch.End();

	// Explosions use additive blending so they need to be drawn after the main sprite batch
	spriteBatch.Begin(SpriteSortMode::Deferred, BlendState::Additive);
	for (Explosion* pExplosion : s_explosions) pExplosion->Draw(spriteBatch);
	spriteBatch.End();
}