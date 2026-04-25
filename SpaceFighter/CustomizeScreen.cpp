#include "CustomizeScreen.h"
#include "MainMenuScreen.h"
#include "GameplayScreen.h"
#include "PlayerShip.h" // To access SelectedTexturePath
#include "Game.h"
#include "MenuItem.h"

using namespace KatanaEngine;

CustomizeScreen::CustomizeScreen()
{
    m_currentShipIndex = 0;
    SetTransitionInTime(1);
    SetTransitionOutTime(0.5f);
    Show();
}

void CustomizeScreen::LoadContent(ResourceManager& resourceManager)
{
    // 1. Configure your 4 .png files
    m_shipFiles = {
        "Textures\\SpaceShipDefault.png",
        "Textures\\SpaceShipAttack.png",
        "Textures\\SpaceShipHealth.png",
        "Textures\\SpaceShipSpeed.png"
    };

    // 2. Configure your ship descriptions
    m_shipDescriptions = {
        "Ship 1: Balanced. Ideal for beginners.",
        "Ship 2: Heavy. High durability but very slow.",
        "Ship 3: Explorer. Very fast with weak shots.",
        "Ship 4: Prototype. High power, hard to control."
    };
    m_shipScales = {
		1.0f, // Scale for Ship 1
		0.5f, // Scale for Ship 2
		0.6f, // Scale for Ship 3
		0.3f  // Scale for Ship 4
    }
    
    
    ;

    // Load the font
    Font::SetLoadSize(20, true);
    m_pFont = resourceManager.Load<Font>("Fonts\\Ethnocentric.ttf");

    // Load the initial preview texture
    m_pPreviewTexture = resourceManager.Load<Texture>(m_shipFiles[m_currentShipIndex]);

    // Screen positions
    m_previewPosition = Game::GetScreenCenter() + Vector2(150, -50);
    m_descriptionPosition = Game::GetScreenCenter() + Vector2(50, 80);

    // 3. Create the Menu
    const int COUNT = 3;
    SetDisplayCount(COUNT);

    enum Items { CHANGE, START, BACK };
    std::string text[COUNT] = { "Next Ship", "Start Game", "Back" };

    for (int i = 0; i < COUNT; i++)
    {
        MenuItem* pItem = new MenuItem(text[i]);
        pItem->SetPosition(Vector2(100, 150 + 60 * i));
        pItem->SetFont(m_pFont);
        pItem->SetColor(Color::BLUE);
        pItem->SetSelected(i == 0);
        AddMenuItem(pItem);
    }

    // --- BUTTON LOGIC ---

    // Change Ship
    GetMenuItem(CHANGE)->SetOnSelect([this, &resourceManager]() {
        m_currentShipIndex = (m_currentShipIndex + 1) % 4; // Cycle between 0 and 3

        // Update the global path for the PlayerShip
        PlayerShip::SelectedTexturePath = m_shipFiles[m_currentShipIndex];
		PlayerShip::SelectedScale = m_shipScales[m_currentShipIndex];

        // Update the screen preview image
        m_pPreviewTexture = resourceManager.Load<Texture>(PlayerShip::SelectedTexturePath);
        });

    // Start Game
    GetMenuItem(START)->SetOnSelect([this]() {
        SetOnRemove([this]() { AddScreen(new GameplayScreen()); });
        Exit();
        });

    // Back
    GetMenuItem(BACK)->SetOnSelect([this]() {
        SetOnRemove([this]() { AddScreen(new MainMenuScreen()); });
        Exit();
        });
}

void CustomizeScreen::Update(const GameTime& gameTime)
{
    float alpha = GetAlpha();
    for (MenuItem* pItem : GetMenuItems())
    {
        pItem->SetAlpha(alpha);
        pItem->SetColor(pItem->IsSelected() ? Color::WHITE : Color::BLUE);
    }
    MenuScreen::Update(gameTime);
}

void CustomizeScreen::Draw(SpriteBatch& spriteBatch)
{
    spriteBatch.Begin();

    // Draw the selected ship preview
    if (m_pPreviewTexture)
    {
        // 1. Obtenemos la escala correspondiente a la nave actual
        float currentScale = m_shipScales[m_currentShipIndex];

        // 2. Agregamos el parámetro de escala (Vector2::ONE * currentScale) al final del Draw
        spriteBatch.Draw(m_pPreviewTexture, m_previewPosition, Color::WHITE * GetAlpha(), m_pPreviewTexture->GetCenter(), Vector2::ONE * currentScale);
    } // <--- ¡Asegúrate de tener esta llave de cierre!

    // Draw the ship description
    if (m_pFont)
    {
        std::string desc = m_shipDescriptions[m_currentShipIndex];
        spriteBatch.DrawString(m_pFont, &desc, m_descriptionPosition, Color::WHITE * GetAlpha());
    }

    MenuScreen::Draw(spriteBatch);
    spriteBatch.End();
}