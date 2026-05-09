#include "LaserBeam.h"

LaserBeam::LaserBeam()
{
	// Radio de colisión para compensar que el láser es continuo
	SetCollisionRadius(50);
}

void LaserBeam::Activate(const Vector2 position, const float lifetime)
{
	SetPosition(position);
	m_lifetime = lifetime;
	GameObject::Activate();
}

void LaserBeam::Update(const GameTime& gameTime)
{
	if (IsActive())
	{
		// Tiempo de vida del láser
		m_lifetime -= (float)gameTime.GetElapsedTime();

		// Apagar si el tiempo expiró
		if (m_lifetime <= 0.0f)
		{
			Deactivate();
		}
	}

	GameObject::Update(gameTime);
}

void LaserBeam::Draw(SpriteBatch& spriteBatch)
{
	if (IsActive() && m_pTexture)
	{
		spriteBatch.End();
		spriteBatch.Begin(SpriteSortMode::Deferred, BlendState::Additive);

		// Dibujamos usando X para el grosor y Y para el largo
		spriteBatch.Draw(
			m_pTexture,
			GetPosition(),
			Color::WHITE,
			m_pTexture->GetCenter(),
			Vector2(m_widthScale, m_lengthScale),
			0.0f
		);

		spriteBatch.End();
		spriteBatch.Begin();
	}
}

Vector2 LaserBeam::GetHalfDimensions() const
{
	if (m_pTexture != nullptr)
	{
		// Calculamos la caja de colisión respetando el ancho y el largo individualmente
		Vector2 center = m_pTexture->GetCenter();
		return Vector2(center.X * m_widthScale, center.Y * m_lengthScale);
	}
	return Vector2::ZERO;
}