
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

#include "KatanaEngine.h"
#include "Animation.h"
#include "KatanaEngine.h"


namespace KatanaEngine
{
	Animation::Animation()
	{
		m_secondsPerFrame = 0.6f;
		m_currentFrameTime = m_secondsPerFrame;
		m_currentIndex = 0;
		m_loopCounter = -1;
		m_isPlaying = true;
	}


	Animation::~Animation()
	{
		for (Region* pRegion : m_frames) delete pRegion;
		m_frames.clear();
	}


	void Animation::Update(const GameTime& gameTime)
	{
		if (m_isPlaying)
		{
 			m_currentFrameTime -= gameTime.GetElapsedTime();

			if (m_currentFrameTime <= 0)
			{
				m_currentIndex++;
				m_currentFrameTime = m_secondsPerFrame;

				if (m_currentIndex == m_frames.size())
				{
					if (m_loopCounter > 0) m_loopCounter--;
					else if (m_loopCounter == 0) Stop();
					else m_currentIndex = 0;
				}
			}
		}
	}


	bool Animation::Load(const std::string &path, ResourceManager *pManager)
	{
		std::ifstream fileIn(path.c_str(), std::ifstream::in);

		if (!fileIn.is_open() || !fileIn.good()) return false;
		
		std::string line;
		std::vector<std::string> splitElements;

		bool loadingSpriteSheet = true;
		bool loadingFrameTime = true;

		while (getline(fileIn, line))
		{
			ParseComments(line);
			if (line.empty()) continue;

			if (loadingSpriteSheet)
			{
				m_pTexture = pManager->Load<Texture>(line);
				loadingSpriteSheet = false;
			}
			else if (loadingFrameTime)
			{
				m_secondsPerFrame = atof(line.c_str());
				loadingFrameTime = false;
			}
			else // loading frames
			{
				Split(line, ',', splitElements);

				Region *frame = new Region;
				frame->X = atoi(splitElements[0].c_str());
				frame->Y = atoi(splitElements[1].c_str());
				frame->Width = atoi(splitElements[2].c_str());
				frame->Height = atoi(splitElements[3].c_str());
				m_frames.push_back(frame);
			}
		}

		fileIn.close();
		return true;
	}


	void Animation::SetCurrentFrame(const unsigned int index)
	{
		if (index < m_frames.size())
		{
			m_currentIndex = index;
			m_currentFrameTime = m_secondsPerFrame;
		}
	}


	void Animation::Stop()
	{
		Pause();
		SetCurrentFrame(0);
	}

	Resource *Animation::Clone()
	{
		Animation *clone = new Animation;

		clone->m_pTexture = m_pTexture;
		clone->m_frames = m_frames;
		clone->m_secondsPerFrame = m_secondsPerFrame;
		clone->m_isPlaying = m_isPlaying;
		clone->m_currentFrameTime = m_currentFrameTime;
		clone->m_currentIndex = m_currentIndex;

		return clone;
	}
}