#pragma once
#ifndef BNCUP_ASSET_REGISTER_H_
#define BNCUP_ASSET_REGISTER_H_

#include <Siv3D.hpp>

namespace bncup
{
	struct AssetPackInfo
	{
		String packName;
		Array<AssetName> audioAssetNames;
		Array<AssetName> fontAssetNames;
		Array<AssetName> textureAssetNames;
	};

	class AssetRegister
	{
	public:

		explicit AssetRegister();
		virtual ~AssetRegister();

		void asyncRegist();

		void unregist();

		void reset();

		AssetRegister& addRegistPackFile(FilePathView packFile);

		bool isReady() const;

		const Array<AssetPackInfo>& getPackInfos() const;

	private:

		Array<FilePath> m_packFiles;
		Array<AssetPackInfo> m_packInfos;
		Optional<AsyncTask<bool>> m_packLoadTask;
	};
}

#endif // !BNCUP_ASSET_REGISTER_H_
