#pragma once

#include "KatanaEngine.h"
#include <vector>
#include <string>

using namespace KatanaEngine;

/** @brief The customization menu screen for the game. */
class CustomizeScreen : public MenuScreen
{
public:

	/** @brief Instantiate a customization screen object. */
	CustomizeScreen();
	virtual ~CustomizeScreen() {}

	/** @brief Load the content for the screen.
		@param resourceManager A reference to the game's resource manager,
		used for loading and managing game assets (resources). */
	virtual void LoadContent(ResourceManager& resourceManager);

	/** @brief Update the screen.
		@param gameTime Timing values including time since last update. */
	virtual void Update(const GameTime& gameTime);

	/** @brief Render the screen.
		@param spriteBatch A reference to the game's sprite batch, used for rendering. */
	virtual void Draw(SpriteBatch& spriteBatch);

private:

	Texture* m_pPreviewTexture = nullptr;
	Vector2 m_previewPosition;

	int m_currentShipIndex = 0;
	std::vector<std::string> m_shipFiles;
	std::vector<std::string> m_shipDescriptions;
	std::vector<float> m_shipScales;

	std::vector<Texture*> m_loadedTextures;

	Font* m_pFont = nullptr;
	Vector2 m_descriptionPosition;

};