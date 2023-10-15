#include "AssetRegister.h"
#include "../Common/Common.h"
namespace
{
	bool PackLoadRegist(const Array<FilePath>& files, Array<bncup::AssetPackInfo>& packInfos)
	{
		packInfos.clear();
		for (const auto& file : files)
		{
			JSON jsonDocument = JSON::Load(file);
			if (jsonDocument.isEmpty())
			{
				DEBUG_BREAK(true);
				continue;
			}
			bncup::AssetPackInfo packInfo;

			if (jsonDocument.hasElement(U"packName"))
			{
				packInfo.packName = jsonDocument[U"packName"].getString();
			}

			if (jsonDocument.hasElement(U"audioAssetDatas")
				and jsonDocument[U"audioAssetDatas"].isArray())
			{
				const size_t size = jsonDocument[U"audioAssetDatas"].size();

				for (size_t i : step(size))
				{
					const auto& audioAssetDataDocument = jsonDocument[U"audioAssetDatas"][i];

					std::unique_ptr<AudioAssetData> assetData;
					assetData.reset(new AudioAssetData());

					String assetName = audioAssetDataDocument[U"assetName"].getOr<String>(U"none");

					assetData->path = audioAssetDataDocument[U"path"].getOr<FilePath>(U"");

					if (audioAssetDataDocument.hasElement(U"loopTiming"))
					{
						AudioLoopTiming tmp;
						tmp.beginPos = audioAssetDataDocument[U"loopTiming"][U"beginPos"].getOr<uint64>(0U);
						tmp.endPos = audioAssetDataDocument[U"loopTiming"][U"endPos"].getOr<uint64>(0U);
						assetData->loopTiming = tmp;
					}
					assetData->streaming = audioAssetDataDocument[U"streaming"].getOr<bool>(false);
					assetData->instrument = ToEnum<GMInstrument>(audioAssetDataDocument[U"instrument"].getOr<uint8>(0u));
					assetData->key = audioAssetDataDocument[U"key"].getOr<uint8>(0);
					assetData->noteOn = Duration{ audioAssetDataDocument[U"noteOn"].getOr<double>(0.0) };
					assetData->noteOff = Duration{ audioAssetDataDocument[U"noteOff"].getOr<double>(0.0) };
					assetData->velocity = audioAssetDataDocument[U"velocity"].getOr<double>(0.0);
					assetData->sampleRate = audioAssetDataDocument[U"sampleRate"].getOr<uint32>(Wave::DefaultSampleRate);

					packInfo.audioAssetNames.push_back(assetName);
					AudioAsset::Register(assetName, std::move(assetData));
				}
			}
			if (jsonDocument.hasElement(U"fontAssetDatas")
				and jsonDocument[U"fontAssetDatas"].isArray())
			{
				const size_t size = jsonDocument[U"fontAssetDatas"].size();

				for (size_t i : step(size))
				{
					const auto& fontAssetDataDocument = jsonDocument[U"fontAssetDatas"][i];

					std::unique_ptr<FontAssetData> assetData;
					assetData.reset(new FontAssetData());

					String assetName = fontAssetDataDocument[U"assetName"].getOr<String>(U"none");

					assetData->fontMethod = ToEnum<FontMethod>(fontAssetDataDocument[U"fontMethod"].getOr<uint8>(0u));
					assetData->fontSize = fontAssetDataDocument[U"fontSize"].getOr<int32>(0u);
					assetData->path = fontAssetDataDocument[U"path"].getOr<FilePath>(U"");
					assetData->faceIndex = fontAssetDataDocument[U"faceIndex"].getOr<size_t>(0u);
					assetData->typeface = ToEnum<Typeface>(fontAssetDataDocument[U"typeface"].getOr<uint8>(0u));
					assetData->style = ToEnum<FontStyle>(fontAssetDataDocument[U"style"].getOr<uint8>(0u));

					packInfo.fontAssetNames.push_back(assetName);
					FontAsset::Register(assetName, std::move(assetData));
				}
			}
			if (jsonDocument.hasElement(U"textureAssetDatas")
				and jsonDocument[U"textureAssetDatas"].isArray())
			{
				const size_t size = jsonDocument[U"textureAssetDatas"].size();

				for (size_t i : step(size))
				{
					const auto& textureAssetDataDocument = jsonDocument[U"textureAssetDatas"][i];

					std::unique_ptr<TextureAssetData> assetData;
					assetData.reset(new TextureAssetData());

					String assetName = textureAssetDataDocument[U"assetName"].getOr<String>(U"none");
					assetData->path = textureAssetDataDocument[U"path"].getOr<FilePath>(U"");
					assetData->secondaryPath = textureAssetDataDocument[U"secondaryPath"].getOr<FilePath>(U"");
					{
						assetData->rgbColor.a = textureAssetDataDocument[U"rgbColor"][U"a"].getOr<uint8>(0u);
						assetData->rgbColor.r = textureAssetDataDocument[U"rgbColor"][U"r"].getOr<uint8>(0u);
						assetData->rgbColor.g = textureAssetDataDocument[U"rgbColor"][U"g"].getOr<uint8>(0u);
						assetData->rgbColor.b = textureAssetDataDocument[U"rgbColor"][U"b"].getOr<uint8>(0u);
					}
					assetData->desc = ToEnum<TextureDesc>(textureAssetDataDocument[U"desc"].getOr<uint8>(0u));
					{
						assetData->emoji = Emoji{ textureAssetDataDocument[U"emoji"][U"codePoints"].getOr<String>(U"") };
					}
					{
						Icon::Type type = ToEnum<Icon::Type>(textureAssetDataDocument[U"icon"][U"type"].getOr<uint8>(0u));
						uint32 code = textureAssetDataDocument[U"icon"][U"code"].getOr<uint32>(0u);
						assetData->icon = Icon{ type, code };
					}
					assetData->iconSize = textureAssetDataDocument[U"iconSize"].getOr<int32>(0);

					packInfo.textureAssetNames.push_back(assetName);
					TextureAsset::Register(assetName, std::move(assetData));
				}
			}
			packInfos.push_back(packInfo);
		}
		return true;
	}
}

namespace bncup
{
	AssetRegister::AssetRegister()
		: m_packFiles{}
		, m_packLoadTask{ none }
		, m_packInfos{}
	{
	}

	AssetRegister::~AssetRegister()
	{
	}

	void AssetRegister::asyncRegist()
	{
		m_packLoadTask.emplace(Async(PackLoadRegist, std::cref(m_packFiles), std::ref(m_packInfos)));
	}

	void AssetRegister::unregist()
	{
		for (const auto& packInfo : m_packInfos)
		{
			for (const auto& assetName : packInfo.audioAssetNames)
			{
				AudioAsset::Unregister(assetName);
			}
			for (const auto& assetName : packInfo.fontAssetNames)
			{
				FontAsset::Unregister(assetName);
			}
			for (const auto& assetName : packInfo.textureAssetNames)
			{
				TextureAsset::Unregister(assetName);
			}
		}
	}

	void AssetRegister::reset()
	{
		m_packFiles.clear();
		m_packInfos.clear();
		m_packLoadTask.reset();
	}

	AssetRegister& AssetRegister::addRegistPackFile(FilePathView packFile)
	{
		m_packFiles.emplace_back(packFile);
		return *this;
	}

	bool AssetRegister::isReady() const
	{
		if (not(m_packLoadTask.has_value()))
		{
			return false;
		}
		return m_packLoadTask->isReady();
	}

	const Array<AssetPackInfo>& AssetRegister::getPackInfos() const
	{
		return m_packInfos;
	}
}
