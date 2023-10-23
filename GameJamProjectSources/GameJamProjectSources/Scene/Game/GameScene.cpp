#include "GameScene.h"
#include "../../Common/Common.h"
#include "Map/MapData.h"
#include "Map/MapView.h"
#include "Map/RoomData.h"
#include "../../Unit/Unit.h"

namespace
{
	static const RoundRect ROUNDRECT_STAGENO_AREA =
	{
		20, 20,
		1240, 80, 10
	};

	static const RoundRect ROUNDRECT_MAPVIEW_AREA =
	{
		20, 120,
		1240, 600, 10
	};

	static const RoundRect ROUNDRECT_LOG_AREA =
	{
		20, 740,
		1020, 200, 10
	};

	static const Circle CIRCLE_CONTROLLER_AREA =
	{
		1160, 840, 100
	};
}

namespace bnscup
{
	class GameScene::Impl
	{
		enum class Step
		{
			Assign,
			Idle,
			Pause,
		};

	public:

		Impl(int stageNo);
		~Impl();

		void update();
		void draw() const;

	private:

		void stepAssign();
		void stepIdle();
		void stepPause();

	private:

		Step m_step;
		Camera2D m_camera;
		std::unique_ptr<MapData> m_pMapData;
		std::unique_ptr<MapView> m_pMapView;
		RectF m_cursorRect;

		Array<std::unique_ptr<Unit>> m_units;

	};

	//==================================================
	
	GameScene::Impl::Impl(int stageNo)
		: m_step{ Step::Idle }
		, m_camera{}
		, m_pMapData{ nullptr }
		, m_pMapView{ nullptr }
		, m_cursorRect{}
		, m_units{}
	{
		if (stageNo == 0)
		{
			auto* pMapData = new MapData(
				{
					RoomData{ FromEnum(RoomData::Route::Down), 0 },
					RoomData{ FromEnum(RoomData::Route::UpDown), 0 },
					RoomData{ FromEnum(RoomData::Route::Up), 0 },
				},
				U"dungeon_tileset",
				1, 3, 16
			);
			m_pMapData.reset(pMapData);
		}

		if (m_pMapData)
		{
			auto* pMapView = new MapView(m_pMapData.get());
			m_pMapView.reset(pMapView);
		}
	}

	GameScene::Impl::~Impl()
	{
	}

	void GameScene::Impl::update()
	{
		switch (m_step)
		{
		case Step::Assign:		stepAssign();		break;
		case Step::Idle:		stepIdle();			break;
		case Step::Pause:		stepPause();		break;
		default: DEBUG_BREAK(true); break;
		}
	}

	void GameScene::Impl::draw() const
	{
		ROUNDRECT_STAGENO_AREA.draw(Palette::Darkslategray).drawFrame(2.0, Palette::Darkgray);
		ROUNDRECT_MAPVIEW_AREA.draw(Palette::Darkslategray).drawFrame(2.0, Palette::Darkgray);
		ROUNDRECT_LOG_AREA.draw(Palette::Darkslategray).drawFrame(2.0, Palette::Darkgray);
		CIRCLE_CONTROLLER_AREA.draw(Palette::Darkslategray).drawFrame(2.0, Palette::Darkgray);
		{
			const Transformer2D transformer{ m_camera.createTransformer() };
			if (m_pMapView)
			{
				m_pMapView->draw();
			}
			m_cursorRect.drawFrame(1.0, Palette::Palegreen);
		}
	}

	void GameScene::Impl::stepAssign()
	{
	}

	void GameScene::Impl::stepIdle()
	{
		{
			const Transformer2D transformer{ m_camera.createTransformer() };
		}
	}

	void GameScene::Impl::stepPause()
	{
	}
	
	//==================================================

	GameScene::GameScene(const super::InitData& init)
		: super{ init }
		, m_pImpl{ nullptr }
	{
		m_pImpl.reset(new Impl(getData().stageNo));
	}

	GameScene::~GameScene()
	{
	}

	void GameScene::update()
	{
		if (m_pImpl)
		{
			m_pImpl->update();
		}
	}

	void GameScene::draw() const
	{
		if (m_pImpl)
		{
			m_pImpl->draw();
		}
	}


}
