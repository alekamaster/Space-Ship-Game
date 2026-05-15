#include "GameOverScreen.h"
#include "MainMenuScreen.h"
#include "GameplayScreen.h"
#include "MenuItem.h"
#include <fstream>
#include <algorithm>

GameOverScreen::GameOverScreen(int finalScore, int finalLevel)
	: m_finalScore(finalScore), m_finalLevel(finalLevel)
{
	SetOnRemove([this]() { AddScreen(new MainMenuScreen()); });
	SetTransitionInTime(1);
	SetTransitionOutTime(0.5f);
	Show();
}

void GameOverScreen::LoadContent(ResourceManager& resourceManager)
{
	// Load fonts
	Font::SetLoadSize(30, true);
	m_pFont = resourceManager.Load<Font>("Fonts\\Ethnocentric.ttf");
	Font::SetLoadSize(16, true);
	m_pSmallFont = resourceManager.Load<Font>("Fonts\\Ethnocentric.ttf");

	// started to work on this leaderboard
	m_leaderboard = LoadLeaderboard();

	// Check if new high score
	if (m_leaderboard.size() < 10 || m_finalScore > m_leaderboard.back().score)
	{
		m_isNewHighScore = true;
		
		// Add new entry
		LeaderboardEntry newEntry;
		newEntry.name = "PLAYER";
		newEntry.score = m_finalScore;
		newEntry.level = m_finalLevel;
		m_leaderboard.push_back(newEntry);

		// Sort by score descending
		std::sort(m_leaderboard.begin(), m_leaderboard.end(), 
			[](const LeaderboardEntry& a, const LeaderboardEntry& b) {
				return a.score > b.score;
			});

		// Keep top 10
		if (m_leaderboard.size() > 10)
			m_leaderboard.resize(10);

		SaveLeaderboard(m_leaderboard);
	}

	// Create menu items
	const int COUNT = 2;
	MenuItem* pItem;
	SetDisplayCount(COUNT);

	enum Items { RETRY, MAIN_MENU };
	std::string text[COUNT] = { "Retry", "Main Menu" };

	for (int i = 0; i < COUNT; i++)
	{
		pItem = new MenuItem(text[i]);
		pItem->SetPosition(Vector2(100, 500 + 40 * i));
		pItem->SetFont(m_pSmallFont);
		pItem->SetColor(Color::BLUE);
		pItem->SetSelected(i == 0);
		AddMenuItem(pItem);
	}

	// Retry game
	GetMenuItem(RETRY)->SetOnSelect([this]() {
		SetOnRemove([this]() { AddScreen(new GameplayScreen()); });
		Exit();
	});

	// Back to main menu
	GetMenuItem(MAIN_MENU)->SetOnSelect(std::bind(&GameOverScreen::Exit, this));
}

void GameOverScreen::Update(const GameTime& gameTime)
{
	m_elapsedTime += gameTime.GetElapsedTime();
	
	bool isSelected = false;
	float alpha = GetAlpha();
	float offset = sinf(gameTime.GetTotalTime() * 10) * 5 + 5;

	for (MenuItem* pItem : GetMenuItems())
	{
		pItem->SetAlpha(alpha);
		isSelected = pItem->IsSelected();
		pItem->SetColor(isSelected ? Color::WHITE : Color::BLUE);
		pItem->SetTextOffset(isSelected ? Vector2::UNIT_X * offset : Vector2::ZERO);
	}

	MenuScreen::Update(gameTime);
}

void GameOverScreen::Draw(SpriteBatch& spriteBatch)
{
	spriteBatch.Begin();

	float alpha = GetAlpha();
	Color textColor = Color::WHITE * alpha;
	Color highlightColor = Color::YELLOW * alpha;

	// Title
	std::string title = "GAME OVER";
	Vector2 titlePos = Vector2(Game::GetScreenWidth() / 2.0f - m_pFont->GetTextWidth(title.c_str()) / 2.0f, 80);
	spriteBatch.DrawString(m_pFont, &title, titlePos, textColor);

	// Final stats
	std::string scoreText = "Final Score: " + std::to_string(m_finalScore);
	std::string levelText = "Reached Level: " + std::to_string(m_finalLevel);
	
	Vector2 scorePos = Vector2(Game::GetScreenWidth() / 2.0f - m_pSmallFont->GetTextWidth(scoreText.c_str()) / 2.0f, 140);
	Vector2 levelPos = Vector2(Game::GetScreenWidth() / 2.0f - m_pSmallFont->GetTextWidth(levelText.c_str()) / 2.0f, 170);
	
	spriteBatch.DrawString(m_pSmallFont, &scoreText, scorePos, textColor);
	spriteBatch.DrawString(m_pSmallFont, &levelText, levelPos, textColor);

	if (m_isNewHighScore)
	{
		float pulse = sinf(m_elapsedTime * 5.0f) * 0.3f + 0.7f;
		Color newHighScoreColor = Color::YELLOW * alpha * pulse;
		std::string highScoreMsg = "NEW HIGH SCORE!";
		Vector2 highScorePos = Vector2(Game::GetScreenWidth() / 2.0f - m_pSmallFont->GetTextWidth(highScoreMsg.c_str()) / 2.0f, 200);
		spriteBatch.DrawString(m_pSmallFont, &highScoreMsg, highScorePos, newHighScoreColor);
	}

	// Leaderboard - started.
	std::string leaderboardTitle = "--- TOP 10 ---";
	Vector2 boardTitlePos = Vector2(Game::GetScreenWidth() / 2.0f - m_pSmallFont->GetTextWidth(leaderboardTitle.c_str()) / 2.0f, 250);
	spriteBatch.DrawString(m_pSmallFont, &leaderboardTitle, boardTitlePos, highlightColor);

	float yPos = 280;
	int rank = 1;
	for (const auto& entry : m_leaderboard)
	{
		std::string entryText = std::to_string(rank) + ". " + entry.name + 
			"  Score: " + std::to_string(entry.score) + 
			"  Lvl: " + std::to_string(entry.level);
		
		Color entryColor = (entry.score == m_finalScore && m_isNewHighScore) ? highlightColor : textColor;
		
		Vector2 entryPos = Vector2(Game::GetScreenWidth() / 2.0f - m_pSmallFont->GetTextWidth(entryText.c_str()) / 2.0f, yPos);
		spriteBatch.DrawString(m_pSmallFont, &entryText, entryPos, entryColor);
		
		yPos += 25;
		rank++;
	}

	// Draw menu items
	for (MenuItem* pItem : GetMenuItems())
	{
		pItem->Draw(spriteBatch);
	}

	spriteBatch.End();
}

std::vector<LeaderboardEntry> GameOverScreen::LoadLeaderboard()
{
	std::vector<LeaderboardEntry> entries;
	std::ifstream file("leaderboard.txt");
	
	if (file.is_open())
	{
		LeaderboardEntry entry;
		while (file >> entry.name >> entry.score >> entry.level)
		{
			entries.push_back(entry);
		}
		file.close();
	}
	
	return entries;
}

void GameOverScreen::SaveLeaderboard(const std::vector<LeaderboardEntry>& entries)
{
	std::ofstream file("leaderboard.txt");
	
	if (file.is_open())
	{
		for (const auto& entry : entries)
		{
			file << entry.name << " " << entry.score << " " << entry.level << "\n";
		}
		file.close();
	}
}
