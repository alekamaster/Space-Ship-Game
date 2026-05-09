#include "BioEnemyShip.h"

// Implementaciones mínimas para evitar que la clase sea abstracta.
// Las funciones delegan en la implementación base cuando tiene sentido
// y proporcionan comportamientos por defecto simples.

BioEnemyShip::BioEnemyShip()
{
	// Valor por defecto del tipo (puede ajustarse más tarde)
	SetType(EnemyType::Normal);
}

BioEnemyShip::~BioEnemyShip()
{
}

void BioEnemyShip::SetType(EnemyType type)
{
	// Delegamos al comportamiento por defecto en EnemyShip
	EnemyShip::SetType(type);
}

void BioEnemyShip::Initialize(const KatanaEngine::Vector2 position, const double delaySeconds)
{
	// Llamamos a la inicialización base para aprovechar lógica común
	EnemyShip::Initialize(position, delaySeconds);
}

void BioEnemyShip::Update(const KatanaEngine::GameTime& gameTime)
{
	// Usar la actualización por defecto del EnemyShip
	EnemyShip::Update(gameTime);
}

void BioEnemyShip::Draw(KatanaEngine::SpriteBatch& spriteBatch)
{
	// Dibujado por defecto (si se requiere personalización, modificar aquí)
	EnemyShip::Draw(spriteBatch);
}

KatanaEngine::Vector2 BioEnemyShip::GetHalfDimensions() const
{
	// Proporciona dimensiones por defecto seguras si no hay acceso directo a la textura.
	// Si se desea cálculo preciso, reemplazar con lógica que use recursos accesibles.
	return KatanaEngine::Vector2(16.0f, 16.0f);
}

void BioEnemyShip::Hit(const float damage)
{
	// Delegamos la lógica de recibir daño a la implementación base.
	EnemyShip::Hit(damage);
}