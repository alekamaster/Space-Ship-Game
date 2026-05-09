/*	 ██╗  ██╗  █████╗  ████████╗  █████╗  ███╗   ██╗  █████╗
	 ██║ ██╔╝ ██╔══██╗ ╚══██╔══╝ ██╔══██╗ ████╗  ██║ ██╔══██╗
	 █████╔╝  ███████║    ██║    ███████║ ██╔██╗ ██║ ███████║
	 ██╔═██╗  ██╔══██║    ██║    ██╔══██║ ██║╚██╗██║ ██╔══██║
	 ██║  ██╗ ██║  ██║    ██║    ██║  ██║ ██║ ╚████║ ██║  ██║
	 ╚═╝  ╚═╝ ╚═╝  ╚═╝/\  ╚═╝    ╚═╝  ╚═╝ ╚═╝  ╚═══╝ ╚═╝  ╚═╝
	/vvvvvvvvvvvvvvvvvvv \=========================================,
	`^^^^^^^^^^^^^^^^^^^ /---------------------------------------"
		Katana Engine \/ © 2012 - Shuriken Studios LLC
				http://www.shurikenstudios.com
*/
#pragma once
#include "KatanaEngine.h"

namespace KatanaEngine
{
	class SpriteBatch;
	class Texture;
	class IParticle;

	//	/** @brief Basic particle updater. */
	class ParticleRenderer : public IParticleRenderer
	{

	public:

		/** @brief Draws the particle.
			@param pParticle A pointer to the particle to draw.
			@param spriteBatch A reference to the game's sprite batch, used for rendering. */
		virtual void Draw(IParticle* pParticle, SpriteBatch& spriteBatch) const override; // <--- ¡AQUÍ ESTÁ LA MAGIA! Solo punto y coma.

		/** @brief Sets the texture to use when rendering the particle.
			@param pTexture A pointer to the texture to use. */
		void SetTexture(Texture* pTexture) { m_pTexture = pTexture; }


	private:

		Texture* m_pTexture = nullptr;

	};

}