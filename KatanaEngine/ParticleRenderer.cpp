#include "ParticleRenderer.h"
#include "SpriteBatch.h" // ¡Aquí sí es seguro incluirlo!
#include "Texture.h"
#include "Particle.h"
#include <iostream>

namespace KatanaEngine
{
	void ParticleRenderer::Draw(IParticle* pParticle, SpriteBatch& spriteBatch) const
	{
		if (!m_pTexture) {
			std::cout << "No texture set for particle renderer!\n";
			return;
		}

		Particle* pP = (Particle*)pParticle;
		Vector2 position = pP->GetPosition();
		Color color = pP->GetColor();
		float scale = pP->GetScale();

		spriteBatch.Draw(m_pTexture, position, color, m_pTexture->GetCenter(), Vector2::ONE * scale);
	}
}