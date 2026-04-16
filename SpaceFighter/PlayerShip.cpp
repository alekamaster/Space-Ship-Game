
#include "PlayerShip.h"
#include "Level.h"

void PlayerShip::LoadContent(ResourceManager& resourceManager)
{
	ConfineToScreen();
	SetResponsiveness(0.1);

	m_pTexture = resourceManager.Load<Texture>("Textures\\PlayerShip.png");

	AudioSample* pAudio = resourceManager.Load<AudioSample>("Audio\\Effects\\Laser.wav");
	pAudio->SetVolume(0.5f);
	GetWeapon("Main Blaster")->SetFireSound(pAudio);

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
		//if (input.IsKeyDown(Key::D)) type |= TriggerType::Secondary;
		//if (input.IsKeyDown(Key::S)) type |= TriggerType::Special;

		//// Handle Xbox Controller
		//GamePadState* pState = input.GetGamePadState(0);
		//if (pState->IsConnected)
		//{
		//	// gamepad overrides keyboard input
		//	Vector2 thumbstick = pState->Thumbsticks.Left;
		//	if (thumbstick.LengthSquared() < 0.08f) thumbstick = Vector2::ZERO;
		//	direction = thumbstick;

		//	type = TriggerType::None;
		//	if (pState->Triggers.Right > 0.5f) type |= TriggerType::Primary;
		//	if (pState->Triggers.Left > 0.5f) type |= TriggerType::Secondary;
		//	if (pState->IsButtonDown(Button::Y)) type |= TriggerType::Special;
		//}


		SetDesiredDirection(direction);
		if (type != TriggerType::None) FireWeapons(type);
	}
}


void PlayerShip::Update(const GameTime& gameTime)
{
	// Get the velocity for the direction that the player is trying to go.
	Vector2 targetVelocity = m_desiredDirection * GetSpeed() * gameTime.GetElapsedTime();
	// We can't go from 0-100 mph instantly! This line interpolates the velocity for us.
	m_velocity = Vector2::Lerp(m_velocity, targetVelocity, GetResponsiveness());
	// Move that direction
	TranslatePosition(m_velocity *2);

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
			// move the ship to the left edge of the screen (keep Y the same)
			SetPosition(Left + GetHalfDimensions().X, pPosition->Y);
			m_velocity.X = 0; // remove any velocity that could potentially
							  // keep the ship pinned against the edge
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
		const float alpha = GetCurrentLevel()->GetAlpha();
		spriteBatch.Draw(m_pTexture, GetPosition(), Color::WHITE * alpha, m_pTexture->GetCenter());
	}
}


Vector2 PlayerShip::GetHalfDimensions() const
{
	return m_pTexture->GetCenter();
}

void PlayerShip::SetResponsiveness(const float responsiveness)
{
	m_responsiveness = Math::Clamp(0, 1, responsiveness);
}
