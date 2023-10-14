# include "DebugPlayer.h"

namespace bncup
{

	DebugPlayer::DebugPlayer()
	{
	}

	DebugPlayer::~DebugPlayer()
	{
	}

	void DebugPlayer::refresh()
	{
		m_isStep = false;

		if (m_pauseKey.down())
		{
			m_isPause = true;
		}

		if (m_resumeKey.down())
		{
			m_isPause = false;
		}

		if (m_isPause)
		{
			if (m_transportKey.pressed())
			{
				m_isStep = true;
			}
			if (m_oneStepKey.down())
			{
				m_isStep = true;
			}
		}
	}

	void DebugPlayer::setPauseKey(const Input& key)
	{
		m_pauseKey = key;
	}

	void DebugPlayer::setResumeKey(const Input& key)
	{
		m_resumeKey = key;
	}

	void DebugPlayer::setTransportKey(const Input& key)
	{
		m_transportKey = key;
	}

	void DebugPlayer::setOneStepKey(const Input& key)
	{
		m_oneStepKey = key;
	}

	bool DebugPlayer::isPause() const noexcept
	{
		if (m_isStep)
		{
			return false;
		}
		return m_isPause;
	}

}
