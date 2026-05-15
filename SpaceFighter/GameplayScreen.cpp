#include "GameplayScreen.h"
#include "MainMenuScreen.h"
#include "Level.h"
#include "Level01.h"
#include "GameOverScreen.h"

GameplayScreen::GameplayScreen(const int levelIndex)
	: m_levelIndex(levelIndex)
{
	SetTransitionInTime(1);
	SetTransitionOutTime(3);

	SetOnRemove([this](){ AddScreen(new MainMenuScreen()); });

	Show();
}

void GameplayScreen::LoadContent(ResourceManager& resourceManager)
{
	m_pResourceManager = &resourceManager;
	
	// Load HUD font
	Font::SetLoadSize(16, true);
	m_pHudFont = resourceManager.Load<Font>("Fonts\\Ethnocentric.ttf");
	
	LoadLevel(m_levelIndex);
}

void GameplayScreen::LoadLevel(const int levelIndex)
{
	if (m_pLevel) delete m_pLevel;

	switch (levelIndex)
	{
	case 0: m_pLevel = new Level01(); break;
	}

	m_pLevel->SetGameplayScreen(this);
	m_pLevel->LoadContent(*m_pResourceManager);
}

void GameplayScreen::HandleInput(const InputState& input)
{
	m_pLevel->HandleInput(input);
}

void GameplayScreen::Update(const GameTime& gameTime)
{
	m_pLevel->Update(gameTime);
}

void GameplayScreen::Draw(SpriteBatch& spriteBatch)
{
	spriteBatch.Begin();

	m_pLevel->Draw(spriteBatch);

	spriteBatch.End();
	
	// Draw HUD on top of everything
	if (m_pHudFont && m_pLevel)
	{
		spriteBatch.Begin();
		
		float padding = 20.0f;
		float screenWidth = (float)Game::GetScreenWidth();
		float screenHeight = (float)Game::GetScreenHeight();
		
		std::string scoreText = "Score: " + std::to_string(m_pLevel->GetScore());
		std::string levelText = "Level: " + std::to_string(m_pLevel->GetCurrentLevel());
		
		// Calculate positions (right-aligned, from bottom)
		float scoreWidth = (float)m_pHudFont->GetTextWidth(scoreText.c_str());
		float levelWidth = (float)m_pHudFont->GetTextWidth(levelText.c_str());
		float lineHeight = (float)m_pHudFont->GetLineHeight();
		
		Vector2 levelPos = Vector2(screenWidth - levelWidth - padding, screenHeight - padding - lineHeight);
		Vector2 scorePos = Vector2(screenWidth - scoreWidth - padding, levelPos.Y - lineHeight - 5);
		
		spriteBatch.DrawString(m_pHudFont, &scoreText, scorePos, Color::WHITE);
		spriteBatch.DrawString(m_pHudFont, &levelText, levelPos, Color::WHITE);
		
		spriteBatch.End();
	}
}

void GameplayScreen::GameOver(int finalScore, int finalLevel)
{
	SetOnRemove([finalScore, finalLevel, this]() { 
		AddScreen(new GameOverScreen(finalScore, finalLevel));
	});
	Exit();
}
