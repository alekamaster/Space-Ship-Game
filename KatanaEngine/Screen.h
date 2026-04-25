/* ██╗  ██╗  █████╗  ████████╗  █████╗  ███╗   ██╗  █████╗
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

// --- INCLUDES NECESARIOS AÑADIDOS AQUÍ ---
#include <functional>     // Para std::function
#include "Color.h"        // Para usar Color::WHITE
// -----------------------------------------

namespace KatanaEngine
{
	// Declaraciones anticipadas (Forward Declarations) para que el compilador sepa
	// que estas clases existen en el motor sin necesidad de incluir sus .h enteros
	class ScreenManager;
	class ResourceManager;
	class InputState;
	class GameTime;
	class SpriteBatch;
	class Game;
	class RenderTarget;

	/** @brief Callback function for when Exit() is called on a screen and		it begins to transition out.		@remark This callback is run automatically when Exit() is called.		@see Screen::Exit()		@see OnRemove */
	typedef std::function<void()> OnExit;

	/** @brief Callback function for when a screen has completely exited		and is about to be removed by the screen manager.		@remark This callback will be run before UnloadContent(). Therefore		resourses can be passed before the screen is removed.		@see ScreenManager		@see Screen::UnloadContent()		@see OnExit */
	typedef std::function<void()> OnRemove;

	/** @brief Base class for all game screens and menus. */
	class Screen
	{
		friend class ScreenManager;

	public:

		Screen();
		virtual ~Screen() {}


		/** @brief Called when resources need to be loaded.			@param resourceManager A reference to the game's resource manager,			used for loading and managing game assets (resources). */
		virtual void LoadContent(ResourceManager& resourceManager) {}

		/** @brief Called when resources need to be unloaded. Override this method to unload			any screen-specific resources. */
		virtual void UnloadContent() {}

		/** @brief Called when the game has determined that player input needs to be processed.			@param input The current state of all player input devices. */
		virtual void HandleInput(const InputState& input) {}

		/** @brief Called when the game has determined that screen logic needs to be processed.			@param gameTime A reference to the game time object. */
		virtual void Update(const GameTime& gameTime) = 0;

		/** @brief Called when the game determines it is time to draw a frame.			@param spriteBatch A reference to the game's sprite batch, used for rendering. */
		virtual void Draw(SpriteBatch& spriteBatch) = 0;

		/** @brief Determines if the screen is currently exiting.			@return Returns true if the screen is exiting, false otherwise. */
		virtual bool IsExiting() const { return m_isExiting; }

		/** @brief Gets the overall screen transition alpha value (or opacity).			This is handy for fading screens in and out.			@return Returns the alpha value.			@remark This function is an alias for the protected GetTransitionValue().			@see GetTransitionValue() */
		float GetAlpha() const { return m_transitionValue; }

		/** @brief Gets a pointer to the ScreenManager, for managing game screens.			@return A pointer to the game's ScreenManager instance. */
		virtual ScreenManager* GetScreenManager() { return m_pScreenManager; }

		/** @brief Add a screen to the screen manager.			@param pScreen A pointer to the screen to be added. */
		virtual void AddScreen(Screen* pScreen) const;

		/** @brief Gets a pointer to the Game.			@return A pointer to the game instance. */
		virtual Game* GetGame() const;

		/** @brief Gets a pointer to the ScreenManager, for managing game screens.			@return A pointer to the game's ScreenManager instance. */
		virtual void SetScreenManager(ScreenManager& screenManager);

		/** @brief Tells the screen to transition in. */
		virtual void Show();

		/** @brief Tells the screen to transition out. When the screen has completed			its transition, UnloadContent() will be called, and the ScreenManager			will remove it.			@remark SetExitCallback() and SetRemoveCallback() can be used to trigger			events involving the exiting process. */
		virtual void Exit();

		/** @brief Sets the callback function for when the Exit() is called.			@param callback The callback function. */
		virtual void SetOnExit(OnExit callback) { m_onExit = callback; }

		/** @brief Sets the callback function for when the screen is about to be			removed by the screen manager.			@param callback The callback function. */
		virtual void SetOnRemove(OnRemove callback) { m_onRemove = callback; }

		/** @brief Get the time in seconds that the screen will transition out.			@return Returns the transition time in seconds. */
		virtual double GetTransitionOutTime() const { return m_transitionOutTime; }


	protected:

		/** @brief Defines the state of the transition between screens. */
		enum class ScreenTransition
		{
			None,	/**< @brief The screen is not transitioning. */
			In,		/**< @brief The screen is transitioning in. */
			Out		/**< @brief The screen is transitioning out. */
		};

		/** @brief Checks if the screen manager should call HandleInput on the			screen below this one.			@return Returns true if HandleInput should be called, false otherwise. */
		virtual bool ShouldHandleInputBelow() { return false; }

		/** @brief Checks if the screen manager should call Update on the			screen below this one.			@return Returns true if Update should be called, false otherwise.			@see Update()*/
		virtual bool ShouldUpdateBelow() { return false; }

		/** @brief Checks if the screen manager should call Draw on the			screen below this one.			@return Returns true if Draw should be called, false otherwise. */
		virtual bool ShouldDrawBelow() { return false; }

		/** @brief Set the time in seconds that the screen should transition in.			@param seconds The transition time. */
		virtual void SetTransitionInTime(double seconds) { m_transitionInTime = seconds; }

		/** @brief Set the time in seconds that the screen should transition out.			@param seconds The transition time. */
		virtual void SetTransitionOutTime(double seconds) { m_transitionOutTime = seconds; }

		/** @brief Get the time in seconds that the screen will transition in.			@return Returns the transition time in seconds. */
		virtual double GetTransitionInTime() const { return m_transitionInTime; }

		/** @brief Get the current transition state of the screen.			@return Returns the current transition. */
		virtual ScreenTransition GetTransition() const { return m_transition; }

		/** @brief Get the interpolated value (between zero and one) of the screen transition.			@return Returns a value that represents the percentage of the transition. Zero means			the screen is entirely transitioned of, and one means it's entirely on. */
		virtual float GetTransitionValue() const { return m_transitionValue; }

		/** @brief Determines if the screen has completely faded out and needs to be removed by			the ScreenManager.			@return Returns true if the screen needs to be removed, false otherwise. */
		virtual bool NeedsToBeRemoved() const { return m_needsToBeRemoved; }

		/** @brief Forces all screen rendering to a render target. */
		virtual void UseRenderTarget();

		/** @brief Get screen's render target.			@return Returns a pointer to the render target, or nullptr if the screen doesn't			use one. */
		virtual RenderTarget* GetRenderTarget() const { return m_pRenderTarget; }

		/** @brief Get the color that will be used to tint the render target.			@return Returns the render target draw tint. */
		virtual Color GetRenderTargetColor() const { return Color::WHITE * GetAlpha(); }


	private:

		OnExit m_onExit;
		OnRemove m_onRemove;

		bool m_isExiting = false;

		bool m_needsToBeRemoved = false;

		double m_transitionInTime = 0;
		double m_transitionOutTime = 0;

		ScreenTransition m_transition = ScreenTransition::None;

		float m_transitionValue = 0;
		double m_transitionTime = 0;

		ScreenManager* m_pScreenManager = nullptr;

		RenderTarget* m_pRenderTarget = nullptr;

		void TransitionIn();
		void TransitionOut();

		void UpdateTransition(const GameTime& gameTime);
	};
}