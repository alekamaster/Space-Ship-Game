#include "LaserBeam.h"

LaserBeam::LaserBeam()
{
	//large radius for the laser beam, since it is a continuous beam that can hit the player from a distance
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
		//time for the laser
		m_lifetime -= (float)gameTime.GetElapsedTime();

		// despawn the laser beam if its lifetime has expired
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
		// draw the laser beam with full opacity, since it is a continuous beam that can hit the player from a distance
		spriteBatch.Draw(m_pTexture, GetPosition(), Color::WHITE, m_pTexture->GetCenter());
	}
}