#pragma once
#ifndef BNSCUP_DEBUG_PLAYER_H_
#define BNSCUP_DEBUG_PLAYER_H_
#include <Siv3D.hpp>

namespace bnscup
{
	/**
	 * @brief デバッグ用再生制御クラス
	 */
	class DebugPlayer
	{
	public:
		/**
		 * @brief Construct a new Debug Player object
		 */
		explicit DebugPlayer();
		
		/**
		 * @brief Destroy the Debug Player object
		 */
		~DebugPlayer();
		
		/**
		 * @brief 
		 */
		void refresh();
		
		/**
		 * @brief Set the Pause Key object 
		 * @param key 
		 */
		void setPauseKey(const Input& key);
		
		/**
		 * @brief Set the Resume Key object 
		 * @param key 
		 */
		void setResumeKey(const Input& key);
		
		/**
		 * @brief Set the Transport Key object 
		 * @param key 
		 */
		void setTransportKey(const Input& key);
		
		/**
		 * @brief Set the One Step Key object 
		 * @param key 
		 */
		void setOneStepKey(const Input& key);
		
		/**
		 * @brief 
		 * @return true 
		 * @return false 
		 */
		bool isPause() const noexcept;
		
	private:
		
		Input m_pauseKey{ KeyPause }; //!< 
		Input m_resumeKey{ KeyPause }; //!< 
		Input m_transportKey{ KeyP }; //!< 
		Input m_oneStepKey{ KeyO }; //!< 
		bool  m_isPause{ false }; //!< 
		bool  m_isStep{ false }; //!< 
	};
}
#endif //!BNSCUP_DEBUG_PLAYER_H_
