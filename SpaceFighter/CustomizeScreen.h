#pragma once
#include "MenuScreen.h"
#include "ResourceManager.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "Font.h"
#include "Vector2.h"
#include "GameTime.h"
#include <vector>
#include <string>

class CustomizeScreen : public KatanaEngine::MenuScreen
{
public:
    CustomizeScreen();

    virtual void LoadContent(KatanaEngine::ResourceManager& resourceManager) override;
    virtual void Update(const KatanaEngine::GameTime& gameTime) override;
    virtual void Draw(KatanaEngine::SpriteBatch& spriteBatch) override;

private:

    // En la sección private de CustomizeScreen.h
    

    // Variables para las texturas y selección
    KatanaEngine::Texture* m_pPreviewTexture;
    KatanaEngine::Vector2 m_previewPosition;

    int m_currentShipIndex;
    std::vector<std::string> m_shipFiles;
    std::vector<std::string> m_shipDescriptions;
    std::vector<float> m_shipScales;

    // Fuente y posición del texto
    KatanaEngine::Font* m_pFont;
    KatanaEngine::Vector2 m_descriptionPosition;
};