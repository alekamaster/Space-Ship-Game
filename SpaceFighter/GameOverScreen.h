#pragma once

#include "KatanaEngine.h"

using namespace KatanaEngine;

// Simple leaderboard entry
struct LeaderboardEntry
{
	std::string name;
	int score;
	int level;
};

/** @brief Game over screen with leaderboard */
class GameOverScreen : public MenuScreen
{
public:
	GameOverScreen(int finalScore, int finalLevel);
	virtual ~GameOverScreen() { }

	virtual void LoadContent(ResourceManager& resourceManager);
	virtual void Update(const GameTime& gameTime);
	virtual void Draw(SpriteBatch& spriteBatch);

	// Load/save leaderboard from file
	static std::vector<LeaderboardEntry> LoadLeaderboard();
	static void SaveLeaderboard(const std::vector<LeaderboardEntry>& entries);

private:
	int m_finalScore;
	int m_finalLevel;
	
	Font* m_pFont = nullptr;
	Font* m_pSmallFont = nullptr;
	
	std::vector<LeaderboardEntry> m_leaderboard;
	bool m_isNewHighScore = false;
	
	float m_elapsedTime = 0.0f;
};