#include "CustomizeScreen.h"
#include "MainMenuScreen.h"
#include "GameplayScreen.h"
#include "PlayerShip.h"
#include "MenuItem.h" // ¡Importante incluir MenuItem!

CustomizeScreen::CustomizeScreen()
{
	m_currentShipIndex = 0;

	// Configurar transiciones igual que en MainMenuScreen
	SetTransitionInTime(1);
	SetTransitionOutTime(0.5f);

	Show(); // important!!
}

void CustomizeScreen::LoadContent(ResourceManager& resourceManager)
{
	// 1. Cargar datos de las naves
	m_shipFiles.push_back("Textures\\SpaceShipDefault.png");
	m_shipDescriptions.push_back("Balanced Ship: \nAverage speed, standard damage.");
	m_shipScales.push_back(1.0f);

	m_shipFiles.push_back("Textures\\SpaceShipHealth.png");
	m_shipDescriptions.push_back("Heavy Ship: \nMore health, slower movement.");
	m_shipScales.push_back(1.1f);

	m_shipFiles.push_back("Textures\\SpaceShipSpeed.png");
	m_shipDescriptions.push_back("Speed Ship: \nHigh mobility, light armor.");
	m_shipScales.push_back(0.5f);

	m_shipFiles.push_back("Textures\\SpaceShipAttack.png");
	m_shipDescriptions.push_back("Elite Ship: \nLethal shot, slow reload.");
	m_shipScales.push_back(0.8f);

	// Precargar las texturas
	for (const std::string& file : m_shipFiles)
	{
		m_loadedTextures.push_back(resourceManager.Load<Texture>(file));
	}

	m_previewPosition = Game::GetScreenCenter() + Vector2(200, -50);
	m_descriptionPosition = m_previewPosition + Vector2(-150, 120);

	// 2. Configurar la fuente (Idéntico a MainMenuScreen)
	Font::SetLoadSize(20, true);
	m_pFont = resourceManager.Load<Font>("Fonts\\Ethnocentric.ttf");

	// 3. Crear los elementos del menú
	const int COUNT = 3;
	MenuItem* pItem;

	SetDisplayCount(COUNT);

	enum Items { CHANGE_SHIP, START_GAME, BACK };

	std::string text[COUNT] = { "Change Ship", "Start Game", "Back" };

	for (int i = 0; i < COUNT; i++)
	{
		pItem = new MenuItem(text[i]);
		pItem->SetPosition(Vector2(100, 100 + 50 * i));
		pItem->SetFont(m_pFont);
		pItem->SetColor(Color::BLUE);
		pItem->SetSelected(i == 0);
		AddMenuItem(pItem);
	}

	// 4. Asignar los delegados (Delegates) usando SetOnRemove y Exit

	// Botón "Change Ship"
	GetMenuItem(CHANGE_SHIP)->SetOnSelect([this]() {
		m_currentShipIndex++;
		if (m_currentShipIndex >= m_shipFiles.size())
		{
			m_currentShipIndex = 0;
		}
		});

	// Botón "Start Game"
	GetMenuItem(START_GAME)->SetOnSelect([this]() {
		PlayerShip::SetSelectedShip(m_currentShipIndex);

		SetOnRemove([this]() { AddScreen(new GameplayScreen()); });
		Exit();
		});

	// Botón "Back"
	GetMenuItem(BACK)->SetOnSelect([this]() {
		SetOnRemove([this]() { AddScreen(new MainMenuScreen()); });
		Exit();
		});
}

void CustomizeScreen::Update(const GameTime& gameTime)
{
	bool isSelected = false;
	float alpha = GetAlpha();
	float offset = sinf(gameTime.GetTotalTime() * 10) * 5 + 5;

	// Actualizar animación de los botones (Idéntico a MainMenuScreen)
	for (MenuItem* pItem : GetMenuItems())
	{
		pItem->SetAlpha(alpha);
		isSelected = pItem->IsSelected();
		pItem->SetColor(isSelected ? Color::WHITE : Color::BLUE);
		pItem->SetTextOffset(isSelected ? Vector2::UNIT_X * offset : Vector2::ZERO);
	}

	// Actualizar la textura previa
	if (!m_loadedTextures.empty())
	{
		m_pPreviewTexture = m_loadedTextures[m_currentShipIndex];
	}

	MenuScreen::Update(gameTime);
}

void CustomizeScreen::Draw(SpriteBatch& spriteBatch)
{
	spriteBatch.Begin();

	float alpha = GetAlpha();

	// Dibujar la textura de la nave con el Alpha global de la pantalla
	if (m_pPreviewTexture)
	{
		float scale = m_shipScales[m_currentShipIndex];
		spriteBatch.Draw(
			m_pPreviewTexture,
			m_previewPosition,
			Color::WHITE * alpha,
			m_pPreviewTexture->GetCenter(),
			Vector2(scale, scale)
		);
	}

	// Dibujar el texto descriptivo con el Alpha global de la pantalla
	if (m_pFont)
	{
		spriteBatch.DrawString(
			m_pFont,
			&m_shipDescriptions[m_currentShipIndex],
			m_descriptionPosition,
			Color::WHITE * alpha
		);
	}

	// Dejar que el menú base dibuje los botones (Idéntico a MainMenuScreen)
	MenuScreen::Draw(spriteBatch);

	spriteBatch.End();
}