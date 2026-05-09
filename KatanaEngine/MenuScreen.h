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

// --- AQUÍ ESTÁN LOS INCLUDES QUE FALTABAN ---
#include "Screen.h"          // Necesario porque esta clase hereda de Screen
#include <vector>            // Necesario para std::vector<MenuItem *>
#include <functional>        // Necesario para std::function (el typedef)
#include <string>            // Buena práctica general para menús
// --------------------------------------------

namespace KatanaEngine
{
	class SpriteBatch;
	class MenuItem;   // <-- Declaración anticipada añadida para que reconozca MenuItem
	class InputState; // <-- Declaración anticipada para HandleInput
	class GameTime;   // <-- Declaración anticipada para Update

	/** @brief Base class for all game menu screens. */
	class MenuScreen : public Screen
	{

	public:

		typedef std::function<void()> OnSelectedIndexChanged;

		MenuScreen();
		virtual ~MenuScreen();

		/** @brief Called when the game has determined that player input needs to be processed.			@param input The current state of all player input devices. */
		virtual void HandleInput(const InputState& input);

		/** @brief Called when the game has determined that screen logic needs to be processed.			@param gameTime A reference to the game time object. */
		virtual void Update(const GameTime& gameTime);

		/** @brief Called when the game determines it is time to draw a frame.			@param spriteBatch A reference to the game's sprite batch, used for rendering. */
		virtual void Draw(SpriteBatch& spriteBatch);

		/** @brief Sets whether scrolling past the end of the menu returns the selection back			to the first item.			@param wraps Pass true if you want the menu items to wrap. */
		virtual void SetItemListWrapping(const bool wraps) { m_itemListWraps = wraps; }

		/** @brief Sets the callback function for when the selected menu item changes.			@param callback The callback function. */
		virtual void SetOnSelectedIndexChanged(OnSelectedIndexChanged callback) { m_onSelectedIndexChanged = callback; }


	protected:

		/** @brief Adds a menu item to the screen.			@param pItem A pointer to the item that will be added. */
		virtual void AddMenuItem(MenuItem* pItem);

		/** @brief Get the currently selected menu item.			@return Returns a pointer to the currently selected menu item. */
		virtual MenuItem* GetSelectedItem() const { return m_menuItems[m_selectedItemIndex]; }

		/** @brief Get a menu item by providing it's index.			@return Returns a pointer to a menu item. */
		virtual MenuItem* GetMenuItem(const int itemIndex) const { return m_menuItems[itemIndex]; }

		/** @brief Set the selected menu item by providing it's index.			@param itemIndex The index of the menu item. */
		virtual void SetSelectedItem(const int itemIndex) { m_selectedItemIndex = itemIndex; }

		/** @brief Set how many menu items to display.			@param count The number of menu items to display. */
		virtual void SetDisplayCount(const unsigned int count) { m_displayCount = count; }

		/** @brief Get the number of menu items that are set to display.			@return Returns the number of displayed menu items. */
		virtual int GetDisplayCount() const { return m_displayCount; }

		/** @brief Get the total number of menu items.			@return Returns the total number of menu items. */
		virtual size_t GetCount() const { return m_menuItems.size(); }

		/** @brief Get the index corresponding to the first displayed menu item.			@return Returns index of the first displayed menu item. */
		virtual int GetDisplayStartIndex() const { return m_displayStartIndex; }

		/** @brief Get the menu items.			@return Returns the vector of menu items. */
		virtual std::vector<MenuItem*>& GetMenuItems() { return m_menuItems; }


	private:

		OnSelectedIndexChanged m_onSelectedIndexChanged;

		std::vector<MenuItem*> m_menuItems;

		unsigned int m_selectedItemIndex = 0;

		unsigned int m_displayCount = 0;

		unsigned int m_displayStartIndex = 0;

		bool m_itemListWraps = false;

	};
}