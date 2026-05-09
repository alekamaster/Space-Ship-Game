
/*
     ██╗  ██╗  █████╗  ████████╗  █████╗  ███╗   ██╗  █████╗ 
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
#include "Resource.h"
#include <string>
#include <map>
#include <vector>


namespace KatanaEngine
{
	/** @brief Loads and manages the lifespan of objects from external files. */
	class ResourceManager
	{

	public:

		ResourceManager() { }

		/** @brief Sets the location of the folder where game resources are stored.
			@param path The file path to the content folder. */
		void SetContentPath(const std::string &path) { m_contentPath = path; }

		/** @brief Unloads all game resources. */
		void UnloadAllResources()
		{
			for (const auto& pair : m_resources) delete pair.second;
			m_resources.clear();

			for (Resource* pClone : m_clones) delete pClone;
			m_clones.clear();
		}

		/** @brief Load and manage a resource.
			@param path The path to the resource.
			@param cache Sets whether the resource manager will manage the resource.
			@param appendContentPath Allows the use of shorter paths when loading assets.
			@remark	Usually a game's content is kept in a single folder which is set by calling
			SetContentPath. This allows you to use shorter paths when loading assets. Occasionally
			a resource is outside of this folder and not using the content path is more convenient. */
		template <typename T>
		T *Load(const std::string &path, const bool cache = true, const bool appendContentPath = true)
		{
			if (m_resources.find(path) != m_resources.end())
			{
				T *pResource = dynamic_cast<T *>(m_resources[path]);

				if (pResource->IsCloneable())
				{
					T *pClone = dynamic_cast<T*>(pResource->Clone());

					pClone->m_id = m_nextResourceID;
					m_nextResourceID++;

					m_clones.push_back(pClone);

					return pClone;
				}

				return pResource;
			}

			T *pT = new T;

			pT->m_pResourceManager = this;

			std::string fullPath;

			if (appendContentPath)
			{
				fullPath = m_contentPath;
				fullPath.append(path);
			}
			else fullPath = path;

			if (pT->Load(fullPath, this))
			{
				if (cache) m_resources[path] = pT;

				pT->m_id = m_nextResourceID;
				m_nextResourceID++;

				return pT;
			}

			delete pT;
			return nullptr;
		}

	private:

		std::map<std::string, Resource *> m_resources;

		std::vector<Resource *> m_clones;

		std::string m_contentPath;

		unsigned short m_nextResourceID = 0;

	};
}