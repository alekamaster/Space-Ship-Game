/*	 ██╗  ██╗  █████╗  ████████╗  █████╗  ███╗   ██╗  █████╗
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

// 1. Incluimos SOLO lo que MenuItem necesita estrictamente
#include <string>
#include <functional>
#include "Vector2.h"
#include "Color.h"
#include "SpriteBatch.h"

namespace KatanaEngine
{
	// 2. Declaraciones anticipadas (Forward Declarations) para romper el ciclo infinito
	class MenuScreen;
	class SpriteBatch;
	class Font;
	class GameTime;

	

	/** @brief Callback function for when a menu item is selected.		@see MenuItem */
	typedef std::function<void()> OnSelect;

	/** @brief Class for menu items contained in a MenuScreen. */
	class MenuItem
	{
		friend class MenuScreen;

	public:

		/** @brief Instantiate a menu item. */
		MenuItem() {}

		/** @brief Instantiate a menu item.			@param text The text that the item will display.			@overload */
		MenuItem(const std::string& text) : m_text(text) {}

		virtual ~MenuItem() {}

		/** @brief Called when the game has determined that screen logic needs to be processed.			@param gameTime A reference to the game time object. */
		virtual void Update(const GameTime& gameTime) {}

		/** @brief Called when the game determines it is time to draw a frame.			@param spriteBatch A reference to the game's sprite batch, used for rendering. */
		virtual void Draw(SpriteBatch& spriteBatch);

		/** @brief Called when the picks this menu option.			@param pMenuScreen A pointer to the menu screen. */
		virtual void Select(MenuScreen* pMenuScreen);

		/** @brief Sets the text of the menu item.			@param text The text. */
		virtual void SetText(const std::string& text) { m_text = text; }

		/** @brief Sets the offset from the items normal position. Usually used to animate the			currently selected menu item.			@param offset The offset. */
		virtual void SetTextOffset(const Vector2 offset) { m_textOffset = offset; }

		/** @brief Determines whether or not this menu item is the one that is currently selected.			@return Returns true if this item is the currently selected one, false otherwise. */
		virtual bool IsSelected() const { return m_isSelected; }

		/** @brief Sets whether or not this menu item is the one that is currently selected.			@param isSelected Pass true to make this item the selected one, and false to when			you no longer want it selected. */
		virtual void SetSelected(const bool isSelected) { m_isSelected = isSelected; }

		/** @brief Determines whether or not this menu item is currently displayed.			@return Returns true if this item is currently displayed, false otherwise. */
		virtual bool IsDisplayed() const { return m_isDisplayed; }

		/** @brief Sets whether or not this menu item is currently displayed.			@param isDisplayed Pass true to display this item, and false to hide it. */
		virtual void SetDisplayed(const bool isDisplayed) { m_isDisplayed = isDisplayed; }

		/** @brief Sets the font of the menu item.			@param pFont A pointer to the font. */
		virtual void SetFont(Font* pFont) { m_pFont = pFont; }

		/** @brief Sets the screen position of the menu item.			@param position The new position. */
		virtual void SetPosition(const Vector2 position) { m_position = position; }

		/** @brief Sets the color of the menu item.			@param color The color. */
		virtual void SetColor(const Color color) { m_color = color; }

		/** @brief Sets the alpha value (opacity) of the menu item.			@param alpha The alpha value. */
		virtual void SetAlpha(const float alpha) { m_alpha = alpha; }

		/** @brief Sets the text alignment for the menu item.			@param textAlign The text alignment. */
		virtual void SetTextAlign(TextAlign textAlign) { m_textAlign = textAlign; }

		/** @brief Sets containing menu screen.			@param pMenuScreen A pointer to the menu screen.			@remark This should not be called manually. It is called automatically when an			item is added to a menu screen via MenuScreen::AddMenuItem().			@see MenuScreen::AddMenuItem() */
		virtual void SetMenuScreen(MenuScreen* pMenuScreen) { m_pMenuScreen = pMenuScreen; }

		/** @brief Sets the callback function for when the menu item is selected.			@param callback The callback function. */
		virtual void SetOnSelect(OnSelect callback) { m_onSelect = callback; }

		/** @brief Gets the index of the menu item.			@return Returns the menu item's index. */
		virtual int GetIndex() const { return m_index; }


	protected:

		/** @brief Sets the index of the menu item.			@param index The index value. */
		virtual void SetIndex(const int index) { m_index = index; }

		/** @brief Gets the menu screen that contains this item.			@return Returns a pointer to the menu screen. */
		virtual MenuScreen* GetMenuScreen() const { return m_pMenuScreen; }

		/** @brief Gets the position of the menu item.			@return Returns the menu item's position on the screen. */
		virtual Vector2 GetPosition() const { return m_position; }

		/** @brief Gets the text of the menu item.			@return Returns the menu item's text. */
		virtual const std::string& GetText() const { return m_text; }


	private:

		int m_index = 0;

		std::string m_text = "Menu Item";

		OnSelect m_onSelect;

		bool m_isSelected = false;

		bool m_isDisplayed = true;

		Font* m_pFont = nullptr;

		Color m_color = Color::BLACK;

		float m_alpha = 1;

		Vector2 m_position;

		Vector2 m_textOffset;

		MenuScreen* m_pMenuScreen = nullptr;

		TextAlign m_textAlign = TextAlign::Left;

	};
}