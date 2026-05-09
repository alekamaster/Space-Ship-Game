#include "PlayerShip.h"
#include "Level.h"

// Using the namespace instead of wrapping the entire code block in it
using namespace KatanaEngine;

// --- STATIC VARIABLE INITIALIZATION FOR CUSTOMIZATION ---
std::string PlayerShip::SelectedTexturePath = "Textures\\SpaceShipDefault.png";
float PlayerShip::SelectedScale = 1.0f;
float PlayerShip::SelectedSpeed = 300.0f;
int PlayerShip::SelectedMaxHitPoints = 5;

void PlayerShip::LoadContent(ResourceManager& resourceManager)
{
	ConfineToScreen();
	SetResponsiveness(0.1f);

	// 1. Apply the stats chosen from the customization menu
	SetSpeed(SelectedSpeed);
	SetMaxHitPoints(SelectedMaxHitPoints);

	// 2. Load the dynamic texture based on the player's choice
	m_pTexture = resourceManager.Load<Texture>(SelectedTexturePath);

	float hitBoxRadius = m_pTexture->GetCenter().X * SelectedScale; 
	SetCollisionRadius(hitBoxRadius);
	// 3. Load Audio and Weapons

	AudioSample* pAudio = resourceManager.Load<AudioSample>("Audio\\Effects\\Laser.wav");
	if (pAudio) pAudio->SetVolume(0.5f);

	Weapon* pMainWeapon = GetWeapon("Main Blaster");
	if (pMainWeapon && pAudio) pMainWeapon->SetFireSound(pAudio);

	SetPosition(Game::GetScreenCenter() + Vector2::UNIT_Y * 300);
}

void PlayerShip::Initialize(Level* pLevel, Vector2& startPosition)
{
	SetPosition(startPosition);
}

void PlayerShip::HandleInput(const InputState& input)
{
	if (IsActive())
	{
		Vector2 direction;
		if (input.IsKeyDown(Key::DOWN)) direction.Y++;
		if (input.IsKeyDown(Key::UP)) direction.Y--;
		if (input.IsKeyDown(Key::Right)) direction.X++;
		if (input.IsKeyDown(Key::Left)) direction.X--;

		// Normalize the direction
		if (direction.X != 0 && direction.Y != 0)
		{
			direction *= Math::NORMALIZE_PI_OVER4;
		}

		TriggerType type = TriggerType::None;
		if (input.IsKeyDown(Key::SPACE)) type |= TriggerType::Primary;

		SetDesiredDirection(direction);
		if (type != TriggerType::None) FireWeapons(type);
	}
}

void PlayerShip::Update(const GameTime& gameTime)
{
	// Get the velocity for the direction that the player is trying to go.
	// Note: GetSpeed() now uses the SelectedSpeed configured in LoadContent.
	Vector2 targetVelocity = m_desiredDirection * GetSpeed() * gameTime.GetElapsedTime();

	// We can't go from 0-100 mph instantly! This line interpolates the velocity for us.
	m_velocity = Vector2::Lerp(m_velocity, targetVelocity, GetResponsiveness());

	// Move that direction
	TranslatePosition(m_velocity * 2);

	if (m_isConfinedToScreen)
	{
		const int PADDING = 4; // keep the ship 4 pixels from the edge of the screen
		const int TOP = PADDING;
		const int Left = PADDING;
		const int Right = Game::GetScreenWidth() - PADDING;
		const int BOTTOM = Game::GetScreenHeight() - PADDING;

		Vector2* pPosition = &GetPosition(); // current position (middle of the ship)
		if (pPosition->X - GetHalfDimensions().X < Left) // are we past the left edge?
		{
			SetPosition(Left + GetHalfDimensions().X, pPosition->Y);
			m_velocity.X = 0;
		}
		if (pPosition->X + GetHalfDimensions().X > Right) // right edge?
		{
			SetPosition(Right - GetHalfDimensions().X, pPosition->Y);
			m_velocity.X = 0;
		}
		if (pPosition->Y - GetHalfDimensions().Y < TOP) // top edge?
		{
			SetPosition(pPosition->X, TOP + GetHalfDimensions().Y);
			m_velocity.Y = 0;
		}
		if (pPosition->Y + GetHalfDimensions().Y > BOTTOM) // bottom edge?
		{
			SetPosition(pPosition->X, BOTTOM - GetHalfDimensions().Y);
			m_velocity.Y = 0;
		}
	}

	Ship::Update(gameTime);
}

void PlayerShip::Draw(SpriteBatch& spriteBatch)
{
	if (IsActive())
	{
		// Restore the Alpha so screen transitions look smooth and correct
		const float alpha = GetCurrentLevel()->GetAlpha();


		Vector2 scaleVector = Vector2(SelectedScale, SelectedScale);
		// Use the static scale to draw the ship at the correct customized size
		spriteBatch.Draw(
			m_pTexture,
			GetPosition(),
			Color::WHITE * alpha,
			m_pTexture->GetCenter(),
			Vector2(SelectedScale, SelectedScale), 
			0.0f
		);
	}
}

Vector2 PlayerShip::GetHalfDimensions() const
{
	
	return m_pTexture->GetCenter() * SelectedScale;
}

void PlayerShip::SetResponsiveness(const float responsiveness)
{
	m_responsiveness = Math::Clamp(0, 1, responsiveness);
}

// --- CENTRAL METHOD TO SET CHOSEN SHIP AND STATS ---
void PlayerShip::SetSelectedShip(int shipIndex)
{
	switch (shipIndex)
	{
	case 1: // Ship 2: Heavy
		SelectedTexturePath = "Textures\\SpaceShipHealth.png";
		SelectedScale = 0.8f;
		SelectedSpeed = 150.0f;
		SelectedMaxHitPoints = 6;
		break;

	case 2: // Ship 3: Speed
		SelectedTexturePath = "Textures\\SpaceShipSpeed.png";
		SelectedScale = 0.1f;
		SelectedSpeed = 450.0f;
		SelectedMaxHitPoints = 1;
		break;

	case 3: // Ship 4: Elite / Lethal
		SelectedTexturePath = "Textures\\SpaceShipAttack.png";
		SelectedScale = .8f;
		SelectedSpeed = 250.0f;
		SelectedMaxHitPoints = 2;
		break;

	default: // DEFAULT CASE (Index 0 or any invalid value)
		SelectedTexturePath = "Textures\\SpaceShipDefault.png"; 
		SelectedScale = 1.0f;
		SelectedSpeed = 300.0f;
		SelectedMaxHitPoints = 3;
		break;
	}
}