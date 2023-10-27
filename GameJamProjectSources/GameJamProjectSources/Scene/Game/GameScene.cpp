#include "GameScene.h"
#include "../../Common/Common.h"
#include "Map/MapData.h"
#include "Map/MapView.h"
#include "Map/RoomData.h"
#include "../../Unit/Unit.h"
#include "../../Item/Item.h"
#include "../../Button/Button.h"

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

	static const Circle CIRCLE_CONTROLLER_UP_AREA =
	{
		1160, 770, 40
	};

	static const Circle CIRCLE_CONTROLLER_DOWN_AREA =
	{
		1160, 900, 40
	};

	static const Circle CIRCLE_CONTROLLER_LEFT_AREA =
	{
		1090, 840, 40
	};

	static const Circle CIRCLE_CONTROLLER_RIGHT_AREA =
	{
		1230, 840, 40
	};

	// 描画された最大のアルファ成分を保持するブレンドステートを作成する
	static const BlendState MakeBlendState()
	{
		BlendState blendState = BlendState::Default2D;
		blendState.srcAlpha = Blend::SrcAlpha;
		blendState.dstAlpha = Blend::DestAlpha;
		blendState.opAlpha = BlendOp::Max;
		return blendState;
	}

	Vec2 MapPosToGlobalPos(const Point& mapPos)
	{
		const int32 chipSize = 16;
		const SizeF roomSize{ chipSize * 5, chipSize * 5 };
		return Vec2{ roomSize.x * 0.5 + roomSize.x * mapPos.x, roomSize.y * 0.5 + roomSize.y * mapPos.y };
	}

	Point MapPosFromGlobalPos(const Vec2& pos)
	{
		const int32 chipSize = 16;
		const SizeF roomSize{ chipSize * 5, chipSize * 5 };
		return Point{
			static_cast<int32>(pos.x - roomSize.x * 0.5) / static_cast<int32>(roomSize.x),
			static_cast<int32>(pos.y - roomSize.y * 0.5) / static_cast<int32>(roomSize.y)
		};
	}
}

namespace bnscup
{
	class GameScene::Impl
	{
		enum class Step
		{
			Assign,
			Idle,
			Move,
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
		void stepMove();
		void stepPause();

	private:

		Step m_step;
		Camera2D m_camera;
		std::unique_ptr<MapData> m_pMapData;
		std::unique_ptr<MapView> m_pMapView;
		RenderTexture m_renderTarget;

		Unit* m_pPlayerUnit;
		Array<std::unique_ptr<Unit>> m_units;

		Array<std::unique_ptr<Item>> m_items;

		Texture m_controllerTexture;
		Button m_upControl;
		Button m_downControl;
		Button m_leftControl;
		Button m_rightControl;

	};

	//==================================================
	
	GameScene::Impl::Impl(int stageNo)
		: m_step{ Step::Idle }
		, m_camera{ Vec2::Zero(), 1.0, Camera2DParameters::NoControl() }
		, m_pMapData{ nullptr }
		, m_pMapView{ nullptr }
		, m_renderTarget{
			static_cast<uint32>(ROUNDRECT_MAPVIEW_AREA.rect.size.x)
			, static_cast<uint32>(ROUNDRECT_MAPVIEW_AREA.rect.size.y) }
		, m_pPlayerUnit{ nullptr }
		, m_units{}
		, m_items{}
		, m_controllerTexture{}
		, m_upControl{ CIRCLE_CONTROLLER_UP_AREA }
		, m_downControl{ CIRCLE_CONTROLLER_DOWN_AREA }
		, m_leftControl{ CIRCLE_CONTROLLER_LEFT_AREA }
		, m_rightControl{ CIRCLE_CONTROLLER_RIGHT_AREA }
	{
		// あとでステージ生成クラスとかにまとめたい
		if (stageNo == 0)
		{
			const int32 chipSize = 16;
			const int32 roomCountX = 2;
			const int32 roomCountY = 3;

			// マップの生成
			auto* pMapData = new MapData(
				{
					RoomData{ FromEnum(RoomData::Route::Down)       , FromEnum(RoomData::Route::None) }, RoomData{ FromEnum(RoomData::Route::None), FromEnum(RoomData::Route::None) },
					RoomData{ FromEnum(RoomData::Route::UpRightDown), FromEnum(RoomData::Route::Up)   }, RoomData{ FromEnum(RoomData::Route::Left), FromEnum(RoomData::Route::None) },
					RoomData{ FromEnum(RoomData::Route::Up)         , FromEnum(RoomData::Route::None) }, RoomData{ FromEnum(RoomData::Route::None), FromEnum(RoomData::Route::None) },
				},
				U"dungeon_tileset",
				roomCountX, roomCountY, chipSize
			);
			m_pMapData.reset(pMapData);

			const Point startRoom{ 0, 2 };

			// プレイヤーの生成
			{
				auto* pPlayerUnit = new Unit();
				pPlayerUnit->setPos(MapPosToGlobalPos(startRoom));
				pPlayerUnit->setTexture(U"dungeon_tileset_2");
				pPlayerUnit->setAnimRect(
					{
						{ 0.2s, RectF{ 128, 64, 16, 32 } },
						{ 0.2s, RectF{ 144, 64, 16, 32 } },
						{ 0.2s, RectF{ 160, 64, 16, 32 } },
						{ 0.2s, RectF{ 176, 64, 16, 32 } },
					}
				);
				m_pPlayerUnit = pPlayerUnit;
				m_units.emplace_back(pPlayerUnit);
			}
			// 救助対象ユニット
			{
				auto* pUnit = new Unit();
				pUnit->setPos(MapPosToGlobalPos(Point{ 0, 0 }));
				pUnit->setTexture(U"dungeon_tileset_2");
				pUnit->setAnimRect(
					{
						{ 0.175s, RectF{ 128, 256, 16, 32 } },
						{ 0.175s, RectF{ 144, 256, 16, 32 } },
						{ 0.175s, RectF{ 160, 256, 16, 32 } },
						{ 0.175s, RectF{ 176, 256, 16, 32 } },
					}
				);
				m_units.emplace_back(pUnit);
			}

			// アイテムの生成
			{
				const Point testKeyPos{ 1, 1 };
				auto* pItem = new Item();
				pItem->setPos(MapPosToGlobalPos(testKeyPos));
				pItem->setSrcRect(RectF{ 9 * chipSize, 9 * chipSize, chipSize, chipSize });
				pItem->setTexture(U"dungeon_tileset");
				m_items.emplace_back(pItem);
			}

		}

		// カメラの設定
		{
			m_camera.setScale(1.9);
			m_camera.setTargetScale(1.9);
			m_camera.setCenter(m_pPlayerUnit->getPos());
			m_camera.setTargetCenter(m_pPlayerUnit->getPos());
		}

		if (m_pMapData)
		{
			auto* pMapView = new MapView(m_pMapData.get());
			m_pMapView.reset(pMapView);
		}

		m_controllerTexture = TextureAsset(U"controller_switch");
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
		case Step::Move:		stepMove();			break;
		case Step::Pause:		stepPause();		break;
		default: DEBUG_BREAK(true); break;
		}
	}

	void GameScene::Impl::draw() const
	{
		ROUNDRECT_STAGENO_AREA.draw(Palette::Darkslategray).drawFrame(2.0, Palette::Darkgray);
		ROUNDRECT_MAPVIEW_AREA.draw(Palette::Darkslategray).drawFrame(2.0, Palette::Darkgray);
		ROUNDRECT_LOG_AREA.draw(Palette::Darkslategray).drawFrame(2.0, Palette::Darkgray);

		// コントローラーの表示
		{
			CIRCLE_CONTROLLER_AREA.draw(Palette::Darkslategray).drawFrame(2.0, Palette::Darkgray);

			const ScopedRenderStates2D samplerState{ SamplerState::ClampNearest };
			const int32 chipSize = 16;
			const int32 xCount = static_cast<int32>(m_controllerTexture.width() / chipSize);
			RectF srcRect{ 0, 0, chipSize, chipSize };
			srcRect.setPos(656 % xCount * chipSize, 656 / xCount * chipSize);
			m_controllerTexture(srcRect).scaled(5).drawAt(m_upControl.getCircle().center);
			srcRect.setPos(726 % xCount * chipSize, 726 / xCount * chipSize);
			m_controllerTexture(srcRect).scaled(5).drawAt(m_downControl.getCircle().center);
			srcRect.setPos(761 % xCount * chipSize, 761 / xCount * chipSize);
			m_controllerTexture(srcRect).scaled(5).drawAt(m_leftControl.getCircle().center);
			srcRect.setPos(691 % xCount * chipSize, 691 / xCount * chipSize);
			m_controllerTexture(srcRect).scaled(5).drawAt(m_rightControl.getCircle().center);
		}
		{
			const ScopedRenderTarget2D target{ m_renderTarget.clear(Palette::Black) };
			const ScopedRenderStates2D blend{ SamplerState::ClampNearest, MakeBlendState() };
			const Transformer2D transformer{ m_camera.createTransformer() };
			if (m_pMapView)
			{
				m_pMapView->draw();
			}

			for (const auto& item : m_items)
			{
				if (item)
				{
					item->draw();
				}
			}

			for (const auto& unit : m_units)
			{
				if (unit)
				{
					unit->draw();
				}
			}
		}
		m_renderTarget.rounded(ROUNDRECT_MAPVIEW_AREA.r).drawAt(ROUNDRECT_MAPVIEW_AREA.center());
	}

	void GameScene::Impl::stepAssign()
	{
		m_step = Step::Idle;
	}

	void GameScene::Impl::stepIdle()
	{
		if (m_pPlayerUnit)
		{
			m_camera.setTargetCenter(m_pPlayerUnit->getPos());
		}
		m_camera.update();

		for (auto& unit : m_units)
		{
			if (unit)
			{
				unit->update();
			}
		}

		m_upControl.update();
		if (m_upControl.isSelected(Button::Sounds::Select)) {
			Point mapPos = MapPosFromGlobalPos(m_pPlayerUnit->getPos());
			const auto& nowRoom = m_pMapData->getRoomData(mapPos);
			if (nowRoom.canPassable(RoomData::Route::Up)) {
				if (nowRoom.isLocked(RoomData::Route::Up)) {
					// 鍵がかかっている
				}
				else {
					mapPos.y -= 1;
					const auto& upRoom = m_pMapData->getRoomData(mapPos);
					if (upRoom.canPassable(RoomData::Route::Down)) {
						if (upRoom.isLocked(RoomData::Route::Down)) {
							// 鍵がかかっている
						}
						else {
							// 通れる
							m_pPlayerUnit->setTargetPos(MapPosToGlobalPos(mapPos));
							m_step = Step::Move;
							return;
						}
					}
				}
			}
		}
		m_downControl.update();
		if (m_downControl.isSelected(Button::Sounds::Select)) {
			Point mapPos = MapPosFromGlobalPos(m_pPlayerUnit->getPos());
			const auto& nowRoom = m_pMapData->getRoomData(mapPos);
			if (nowRoom.canPassable(RoomData::Route::Down)) {
				if (nowRoom.isLocked(RoomData::Route::Down)) {
					// 鍵がかかっている
				}
				else {
					mapPos.y += 1;
					const auto& upRoom = m_pMapData->getRoomData(mapPos);
					if (upRoom.canPassable(RoomData::Route::Up)) {
						if (upRoom.isLocked(RoomData::Route::Up)) {
							// 鍵がかかっている
						}
						else {
							// 通れる
							m_pPlayerUnit->setTargetPos(MapPosToGlobalPos(mapPos));
							m_step = Step::Move;
							return;
						}
					}
				}
			}
		}
		m_leftControl.update();
		if (m_leftControl.isSelected(Button::Sounds::Select)) {
			m_pPlayerUnit->setMirror(true);
			Point mapPos = MapPosFromGlobalPos(m_pPlayerUnit->getPos());
			const auto& nowRoom = m_pMapData->getRoomData(mapPos);
			if (nowRoom.canPassable(RoomData::Route::Left)) {
				if (nowRoom.isLocked(RoomData::Route::Left)) {
					// 鍵がかかっている
				}
				else {
					mapPos.x -= 1;
					const auto& upRoom = m_pMapData->getRoomData(mapPos);
					if (upRoom.canPassable(RoomData::Route::Right)) {
						if (upRoom.isLocked(RoomData::Route::Right)) {
							// 鍵がかかっている
						}
						else {
							// 通れる
							m_pPlayerUnit->setTargetPos(MapPosToGlobalPos(mapPos));
							m_step = Step::Move;
							return;
						}
					}
				}
			}
		}
		m_rightControl.update();
		if (m_rightControl.isSelected(Button::Sounds::Select)) {
			m_pPlayerUnit->setMirror(false);
			Point mapPos = MapPosFromGlobalPos(m_pPlayerUnit->getPos());
			const auto& nowRoom = m_pMapData->getRoomData(mapPos);
			if (nowRoom.canPassable(RoomData::Route::Right)) {
				if (nowRoom.isLocked(RoomData::Route::Right)) {
					// 鍵がかかっている
				}
				else {
					mapPos.x += 1;
					const auto& upRoom = m_pMapData->getRoomData(mapPos);
					if (upRoom.canPassable(RoomData::Route::Left)) {
						if (upRoom.isLocked(RoomData::Route::Left)) {
							// 鍵がかかっている
						}
						else {
							// 通れる
							m_pPlayerUnit->setTargetPos(MapPosToGlobalPos(mapPos));
							m_step = Step::Move;
							return;
						}
					}
				}
			}
		}
	}

	void GameScene::Impl::stepMove()
	{
		if (m_pPlayerUnit)
		{
			m_camera.setTargetCenter(m_pPlayerUnit->getPos());
		}
		m_camera.update();

		bool isMoving = false;
		for (auto& unit : m_units)
		{
			if (unit)
			{
				unit->update();
				if (unit->isMoving())
				{
					isMoving = true;
				}
			}
		}
		if (isMoving)
		{
			return;
		}
		m_step = Step::Idle;
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
