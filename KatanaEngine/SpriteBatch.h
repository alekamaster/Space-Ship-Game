
/*
     ██╗  ██╗  █████╗  ████████╗  █████╗  ███╗   ██╗  █████╗ 
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

#include <vector>
#include <string>
#include <allegro5/allegro.h> // <-- Soluciona el error de ALLEGRO_TRANSFORM
#include "Vector2.h"
#include "Color.h"            // <-- Soluciona el error de 'color'
#include "Texture.h"          // <-- Soluciona las confusiones de 'int'
#include "Font.h"
#include "Animation.h"        // <-- Soluciona el error de 'Animation'
#include "Region.h"

namespace KatanaEngine
{
	/** @brief Defines the states for text alignment. */
	enum class TextAlign
	{
		Left,			/**< Align the text to the left. */
		Center,			/**< Align the text to the center. */
		Right			/**< Align the text to the right. */
	};

	/** @brief Defines the methods for sorting sprites before rendering.
		@see SpriteBatch::Begin()
		@see SpriteBatch::Draw() */
	enum class SpriteSortMode
	{
		BackToFront,	/**< Sprites rendered with a lower draw order will render behind those with a higher draw order. */
		Deferred,		/**< Sprites rendered first will appear behind those rendered later. */
		FrontToBack,	/**< Sprites rendered with a higher draw order will render behind those with a lower draw order. */
		Immediate,		/**< Sprites will not be batched, and will draw immediately. */
		Texture			/**< Sprites that share a texture will all be rendered together. */
	};

	/** @brief Defines the way in which textures blending will be calculated. */
	enum class BlendState
	{
		Alpha,			/**< Textures will be blended using premultiplied alpha blending. */
		Additive		/**< Textures will be blended using additive blending. */
	};

	/** @brief Enables a group of sprites to be drawn using the same settings. */
	class SpriteBatch
	{

	public:

		SpriteBatch() { }
		~SpriteBatch() { }

		/** @brief Begins a sprite batch operation.
			@param sortMode Defines how to sort the sprites for rendering.
			@param blendState Defines how to blend overlaping sprites.
			@param pTransformation Defines a screen space transformation to use. */
		void Begin(const SpriteSortMode sortMode = SpriteSortMode::Deferred,
			const BlendState blendState = BlendState::Alpha,
			ALLEGRO_TRANSFORM *pTransformation = NULL);

		/** @brief Flushes the sprite batch and restores the device state to how
		it was before Begin was called. */
		void End(/*bool test = false*/);

		/** @brief Adds a string to a batch of sprites to be rendered.
			@param pFont The font used to draw the text.
			@param text The text to display.
			@param position The screen position of the text.
			@param color The color to tint the text. The default is Color::WHITE (no tint).
			@param alignment The prefered method of aligning the text. The default is
			TextAlign::Left.
			@param drawDepth The depth at which to render the sprite. This is determined
			by the SpriteSortMode arguement that is passed to SpriteBatch::Begin(). The
			default is zero.*/
		void DrawString(const Font *pFont, std::string *text, const Vector2 position,
			const Color color = Color::WHITE, const TextAlign alignment = TextAlign::Left,
			const float drawDepth = 0);

		/** @brief Adds a sprite to a batch of sprites to be rendered. 
			@param pTexture A pointer to the texture to render.
			@param position The screen position of the sprite.
			@param region The region of the texture to render.
			@param color The color to tint the sprite. The default is Color::WHITE (no tint).
			@param origin The sprite's origin. The default is Vector2::Zero, which is the
			upper left corner of the sprite.
			@param scale The scale factor of the sprite. The default is Vector2::One.
			@param rotation The rotation angle in radians. The default is zero.
			@param drawDepth The depth at which to render the sprite. This is determined
			by the SpriteSortMode arguement that is passed to SpriteBatch::Begin(). The
			default is zero.*/
		void Draw(const Texture *pTexture, const Vector2 position, const Region region,
			const Color color = Color::WHITE, const Vector2 origin = Vector2::ZERO, 
			const Vector2 scale = Vector2::ONE, const float rotation = 0, 
			const float drawDepth = 0);

		/** @brief Adds a sprite to a batch of sprites to be rendered.
			@param pTexture A pointer to the texture to render.
			@param position The screen position of the sprite.
			@param color The color to tint the sprite. The default is Color::WHITE (no tint).
			@param origin The sprite's origin. The default is Vector2::Zero, which is the
			upper left corner of the sprite.
			@param scale The scale factor of the sprite. The default is Vector2::One.
			@param rotation The rotation angle in radians. The default is zero.
			@param drawDepth The depth at which to render the sprite. This is determined
			by the SpriteSortMode arguement that is passed to SpriteBatch::Begin(). The
			default is zero.

			@overload */
		void Draw(const Texture *pTexture, const Vector2 position,
			const Color color = Color::WHITE, const Vector2 origin = Vector2::ZERO,
			const Vector2 scale = Vector2::ONE, const float rotation = 0,
			const float drawDepth = 0);

		/** @brief Adds a sprite to a batch of sprites to be rendered.
			@param pAnimation A pointer to the animation to render.
			@param position The screen position of the sprite.
			@param color The color to tint the sprite. The default is Color::WHITE (no tint).
			@param origin The sprite's origin. The default is Vector2::Zero, which is the
			upper left corner of the sprite.
			@param scale The scale factor of the sprite. The default is Vector2::One.
			@param rotation The rotation angle in radians. The default is zero.
			@param drawDepth The depth at which to render the sprite. This is determined
			by the SpriteSortMode arguement that is passed to SpriteBatch::Begin(). The
			default is zero.

			@overload */
		void Draw(Animation *pAnimation, const Vector2 position,
			const Color color = Color::WHITE, const Vector2 origin = Vector2::ZERO,
			const Vector2 scale = Vector2::ONE, const float rotation = 0,
			float drawDepth = 0);

		/** @brief Gets the current settings from the sprite batch.
			@param sortMode Defines how to sort the sprites for rendering.
			@param blendState Defines how to blend overlaping sprites.
			@param pTransformation Defines a screen space transformation to use. */
		void GetBatchSettings(SpriteSortMode &sortMode, BlendState &blendState, ALLEGRO_TRANSFORM *pTransformation);


	private:

		struct Drawable
		{
			bool isBitmap;
			ALLEGRO_COLOR color;
			int x, y;
			float depth;

			union
			{
				struct
				{
					ALLEGRO_FONT *pFont;
					std::string *text;
					TextAlign align;
				};

				struct
				{
					ALLEGRO_BITMAP *pBitmap;
					float rotation;
					int cx, cy;
					int sx, sy, sw, sh;
					float scx, scy;
					unsigned short id;
				};

			} Union;

			bool operator<(const Drawable& other) const
			{
				return (depth < other.depth);
			}
		};

		struct CompareBackToFront
		{
			bool operator()(const Drawable* l, const Drawable* r) { return (*l < *r); }
		};

		struct CompareFrontToBack
		{
			bool operator()(const Drawable* l, const Drawable* r) { return (*r < *l); }
		};

		std::vector<Drawable *> m_drawables;
		std::vector<Drawable *> m_inactiveDrawables;
		std::vector<Drawable *>::iterator m_it;

		unsigned short m_lastID = -1;

		SpriteSortMode m_sortMode = SpriteSortMode::Deferred;

		BlendState m_blendState = BlendState::Alpha;

		ALLEGRO_TRANSFORM *m_pTransformation = nullptr;

		bool m_isStarted = false;

		void DrawBitmap(Drawable *drawable);
		void DrawFont(Drawable *drawable);

	};
}