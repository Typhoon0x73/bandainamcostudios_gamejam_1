#include "GameScene.h"
#include "../../Common/Common.h"
#include "Map/MapData.h"
#include "Map/MapView.h"
#include "Map/RoomData.h"
#include "../../Unit/Unit.h"
#include "../../Item/Item.h"
#include "../../Button/Button.h"
#include "../../MessageBox/MessageBox.h"
#include "../../TeleportAnim/TeleportAnim.h"

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

	static const RectF RECT_EXIT_BUTTON =
	{
		1120, 670, 80, 40,
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
			RescueAnim,
			ReturnAnim,
			UseKeyPopup,
			RescuePopup,
			ReturnPopup,
			Result,
			End,
		};

		struct UnlockRoomData
		{
			RoomData* pTargetRoom;
			RoomData::Route unlockRoute;
		};

		using Rescued = YesNo<struct Rescued_tag>;
		struct RescueUnitData
		{
			Rescued rescued;
			Unit* pTargetUnit;
		};


	public:

		Impl(int stageNo);
		~Impl();

		void update();
		void draw() const;

		bool isEnd() const;
		SceneKey getNextScene() const;

	private:

		void stepAssign();
		void stepIdle();
		void stepMove();
		void stepPause();
		void stepRescueAnim();
		void stepReturnAnim();
		void stepUseKeyPopup();
		void stepRescuePopup();
		void stepReturnPopup();
		void stepResult();

		void createUseKeyPopup();
		void createRescuePopup();
		void createReturnPopup();

	private:

		SceneKey m_nextScene;

		Step m_step;
		Camera2D m_camera;
		std::unique_ptr<MapData> m_pMapData;
		std::unique_ptr<MapView> m_pMapView;
		RenderTexture m_renderTarget;

		Unit* m_pPlayerUnit;
		Array<std::unique_ptr<Unit>> m_units;
		Array<RescueUnitData> m_targetUnits;
		Array<Unit*> m_enemies;

		Array<Item*> m_holdKeys;
		Array<std::unique_ptr<Item>> m_items;

		Texture m_controllerTexture;
		Button m_controlButtons[4];
		Button m_exitButton;

		std::unique_ptr<MessageBox> m_pMessageBox;
		Optional<UnlockRoomData> m_unlockRoomData;
		RescueUnitData* m_pRescueUnitTarget;

		TeleportAnim m_teleportAnim;

		Font m_buttonFont;
		Audio m_collectItemSE;
		Audio m_unlockDoorSE;
	};

	//==================================================
	
	GameScene::Impl::Impl(int stageNo)
		: m_nextScene{ SceneKey::Title }
		, m_step{ Step::Idle }
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
		, m_controlButtons{
			Button(CIRCLE_CONTROLLER_UP_AREA)
			, Button(CIRCLE_CONTROLLER_DOWN_AREA)
			, Button(CIRCLE_CONTROLLER_LEFT_AREA)
			, Button(CIRCLE_CONTROLLER_RIGHT_AREA) }
		, m_exitButton{ RECT_EXIT_BUTTON }
		, m_pMessageBox{ nullptr }
		, m_unlockRoomData{ none }
		, m_pRescueUnitTarget{ nullptr }
		, m_teleportAnim{}
		, m_buttonFont{}
		, m_collectItemSE{}
		, m_unlockDoorSE{}
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
				m_targetUnits.emplace_back(Rescued::No, pUnit);
				m_units.emplace_back(pUnit);
			}

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

			// アイテムの生成
			{
				const Point testKeyPos{ 1, 1 };
				auto* pItem = new Item(Item::Type::GoldKey);
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
		m_buttonFont = FontAsset(U"font_button");
		m_collectItemSE = AudioAsset(U"sd_collect_item");
		m_unlockDoorSE = AudioAsset(U"sd_unlock_door");
	}

	GameScene::Impl::~Impl()
	{
	}

	void GameScene::Impl::update()
	{
		if (m_pPlayerUnit)
		{
			m_camera.setTargetCenter(m_pPlayerUnit->getPos());
		}
		m_camera.update();

		switch (m_step)
		{
		case Step::Assign:		stepAssign();		break;
		case Step::Idle:		stepIdle();			break;
		case Step::Move:		stepMove();			break;
		case Step::Pause:		stepPause();		break;
		case Step::RescueAnim:	stepRescueAnim();	break;
		case Step::ReturnAnim:	stepReturnAnim();	break;
		case Step::UseKeyPopup:	stepUseKeyPopup();	break;
		case Step::RescuePopup:	stepRescuePopup();	break;
		case Step::ReturnPopup:	stepReturnPopup();	break;
		case Step::Result:		stepResult();		break;
		case Step::End:			break;
		default:				DEBUG_BREAK(true);	break; // ステップの処理追加忘れ
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
			static const int32 CONTROLLER_CHIP_TABLE[] = { 656, 726, 761, 691 };
			for (size_t i : step(std::size(m_controlButtons)))
			{
				const int32 chipNo = CONTROLLER_CHIP_TABLE[i];
				srcRect.setPos(chipNo % xCount * chipSize, chipNo / xCount * chipSize);
				const auto& centerPos = m_controlButtons[i].getCircle().center;
				m_controllerTexture(srcRect).scaled(5).drawAt(centerPos);
			}
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
					if (item->existOwer())
					{
						continue;
					}
					item->draw();
				}
			}

			for (const auto& unit : m_units)
			{
				if (unit)
				{
					if (not(unit->isEnable()))
					{
						continue;
					}
					unit->draw();
				}
			}
			m_teleportAnim.draw();
		}
		m_renderTarget.rounded(ROUNDRECT_MAPVIEW_AREA.r).drawAt(ROUNDRECT_MAPVIEW_AREA.center());

		// 脱出ボタン
		{
			const auto& buttonRect = m_exitButton.getRect();
			buttonRect.rounded(3).draw(Palette::Darkred).drawFrame(1.0, Palette::Black);
			m_buttonFont(U"脱出").drawAt(buttonRect.center(), Palette::Black);
		}

		if (m_pMessageBox)
		{
			m_pMessageBox->draw();
		}
	}

	bool GameScene::Impl::isEnd() const
	{
		return (m_step == Step::End);
	}

	SceneKey GameScene::Impl::getNextScene() const
	{
		return m_nextScene;
	}

	void GameScene::Impl::stepAssign()
	{
		m_step = Step::Idle;
	}

	void GameScene::Impl::stepIdle()
	{
		for (auto& unit : m_units)
		{
			if (unit)
			{
				unit->update();
			}
		}

		for (auto& button : m_controlButtons)
		{
			button.update();
		}

		m_exitButton.update();
		if (m_exitButton.isSelected(Button::Sounds::Select))
		{
			createReturnPopup();
			return;
		}

		if (m_pPlayerUnit)
		{
			const auto& playerPos = MapPosFromGlobalPos(m_pPlayerUnit->getPos());
			for (auto& item : m_items)
			{
				if (item->existOwer())
				{
					continue;
				}

				// 鍵との判定
				if (item->isKey())
				{
					const auto& itemPos = MapPosFromGlobalPos(item->getPos());
					if (itemPos == playerPos)
					{
						item->setOwner(m_pPlayerUnit);
						m_holdKeys.emplace_back(item.get());
						m_collectItemSE.playOneShot();
						break;
					}
				}
			}

			for (auto& targetUnit : m_targetUnits)
			{
				// 救助済み
				if (targetUnit.rescued)
				{
					continue;
				}
				const auto& targetPos = MapPosFromGlobalPos(targetUnit.pTargetUnit->getPos());
				if (targetPos == playerPos)
				{
					m_pRescueUnitTarget = &targetUnit;
					createRescuePopup();
					return;
				}
			}
		}

		// 入れ子パラダイス
		// 後で別にまとめる
		// 通れるか、鍵を使用するかの確認
		{
			static const std::pair<RoomData::Route, RoomData::Route> ROUTE_TABLE[] = {
				{ RoomData::Route::Up, RoomData::Route::Down },
				{ RoomData::Route::Down, RoomData::Route::Up },
				{ RoomData::Route::Left, RoomData::Route::Right },
				{ RoomData::Route::Right, RoomData::Route::Left }
			};
			for (size_t i : step(std::size(m_controlButtons)))
			{
				if (m_controlButtons[i].isSelected(Button::Sounds::Select))
				{
					Point mapPos = MapPosFromGlobalPos(m_pPlayerUnit->getPos());
					auto& nowRoom = m_pMapData->getRoomData(mapPos);
					const auto& route = ROUTE_TABLE[i];
					if (route.first == RoomData::Route::Left)
					{
						m_pPlayerUnit->setMirror(true);
					}
					else if (route.first == RoomData::Route::Right)
					{
						m_pPlayerUnit->setMirror(false);
					}
					if (nowRoom.canPassable(route.first))
					{
						if (nowRoom.isLocked(route.first))
						{
							// 鍵がかかっている
							m_unlockRoomData.emplace(&nowRoom, route.first);
							createUseKeyPopup();
							return;
						}
						else
						{
							// 移動先
							switch (route.first)
							{
							case bnscup::RoomData::Route::Up:
								mapPos.y -= 1;
								break;
							case bnscup::RoomData::Route::Right:
								mapPos.x += 1;
								break;
							case bnscup::RoomData::Route::Down:
								mapPos.y += 1;
								break;
							case bnscup::RoomData::Route::Left:
								mapPos.x -= 1;
								break;
							default:
								DEBUG_BREAK(true);
								break;
							}
							auto& targetRoom = m_pMapData->getRoomData(mapPos);
							if (targetRoom.canPassable(route.second))
							{
								if (targetRoom.isLocked(route.second))
								{
									// 鍵がかかっている
									m_unlockRoomData.emplace(&targetRoom, route.second);
									createUseKeyPopup();
									return;
								}
								else
								{
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
		}
		
	}

	void GameScene::Impl::stepMove()
	{
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

	void GameScene::Impl::stepRescueAnim()
	{
		m_teleportAnim.update();
		if (not(m_teleportAnim.isEnd()))
		{
			return;
		}
		m_teleportAnim.reset();
		createReturnPopup();
	}

	void GameScene::Impl::stepReturnAnim()
	{
		m_teleportAnim.update();
		if (not(m_teleportAnim.isEnd()))
		{
			return;
		}
		m_teleportAnim.reset();
		m_step = Step::Result;
	}

	void GameScene::Impl::stepUseKeyPopup()
	{
		if (m_pMessageBox.get() == nullptr)
		{
			DEBUG_BREAK(true); // 処理しようがない。
			m_step = Step::Idle;
			return;
		}

		m_pMessageBox->update();

		if (m_pMessageBox->isYesSelected())
		{
			// アンロック
			if (m_unlockRoomData)
			{
				m_unlockRoomData->pTargetRoom->unlock(m_unlockRoomData->unlockRoute);
				m_unlockDoorSE.playOneShot();
			}
		}
		else if (m_pMessageBox->isNoSelected() or m_pMessageBox->isExitCrossSelected())
		{
			// メッセージキャンセル
		}
		else
		{
			return;
		}

		// 処理終わり
		m_unlockRoomData.reset();
		m_pMessageBox.reset();
		m_step = Step::Idle;
	}

	void GameScene::Impl::stepRescuePopup()
	{
		if (m_pMessageBox.get() == nullptr)
		{
			DEBUG_BREAK(true); // 処理しようがない。
			m_pRescueUnitTarget = nullptr;
			m_step = Step::Idle;
			return;
		}

		m_pMessageBox->update();

		Step nextStep = Step::Idle;
		if (m_pMessageBox->isYesSelected())
		{
			m_pRescueUnitTarget->rescued = Rescued::Yes;
			m_pRescueUnitTarget->pTargetUnit->setEnable(false);
			m_teleportAnim.setPos(m_pRescueUnitTarget->pTargetUnit->getPos());
			m_teleportAnim.reset();
			m_teleportAnim.setEnable(true);
			nextStep = Step::RescueAnim;
		}
		else if (m_pMessageBox->isNoSelected() or m_pMessageBox->isExitCrossSelected())
		{
			// メッセージキャンセル
		}
		else
		{
			return;
		}

		// 処理終わり
		m_pRescueUnitTarget = nullptr;
		m_pMessageBox.reset();
		m_step = nextStep;
	}

	void GameScene::Impl::stepReturnPopup()
	{
		if (m_pMessageBox.get() == nullptr
			or m_pPlayerUnit == nullptr)
		{
			DEBUG_BREAK(true); // 処理しようがない。
			m_step = Step::Idle;
			return;
		}

		m_pMessageBox->update();

		Step nextStep = Step::Idle;
		if (m_pMessageBox->isYesSelected())
		{
			m_teleportAnim.setPos(m_pPlayerUnit->getPos());
			m_teleportAnim.reset();
			m_teleportAnim.setEnable(true);
			m_pPlayerUnit->setEnable(false);
			m_pPlayerUnit = nullptr;
			nextStep = Step::ReturnAnim;
		}
		else if (m_pMessageBox->isNoSelected() or m_pMessageBox->isExitCrossSelected())
		{
			// メッセージキャンセル
		}
		else
		{
			return;
		}

		// 処理終わり
		m_pMessageBox.reset();
		m_step = nextStep;
	}

	void GameScene::Impl::stepResult()
	{
		m_nextScene = SceneKey::StageSelect;
		m_step = Step::End;
		return;
	}

	void GameScene::Impl::createUseKeyPopup()
	{
		auto* pMessageBox = new MessageBox(MessageBox::ButtonStyle::YesNo, MessageBox::ExistCrossButton::No, U"鍵を使用しますか？");
		m_pMessageBox.reset(pMessageBox);
		m_step = Step::UseKeyPopup;
	}

	void GameScene::Impl::createRescuePopup()
	{
		auto* pMessageBox = new MessageBox(MessageBox::ButtonStyle::YesNo, MessageBox::ExistCrossButton::No, U"助けますか？");
		m_pMessageBox.reset(pMessageBox);
		m_step = Step::RescuePopup;
	}

	void GameScene::Impl::createReturnPopup()
	{
		String exitMessage = U"";
		size_t rescueCount = 0;
		for (const auto& targetUnit : m_targetUnits)
		{
			if (targetUnit.rescued)
			{
				rescueCount++;
			}
		}
		if (rescueCount != m_targetUnits.size())
		{
			exitMessage = U"まだ助けていないユニットがいます。\n";
		}
		else
		{
			exitMessage = U"すべてのユニットを救助しました！\n";
		}
		exitMessage += U"脱出しますか？";
		auto* pMessageBox = new MessageBox(MessageBox::ButtonStyle::YesNo, MessageBox::ExistCrossButton::No, exitMessage);
		m_pMessageBox.reset(pMessageBox);
		m_step = Step::ReturnPopup;
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
			if (m_pImpl->isEnd())
			{
				auto& sceneData = getData();
				sceneData.nextScene = m_pImpl->getNextScene();
				changeScene(SceneKey::Load, 1.0s);
			}
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
