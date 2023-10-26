#pragma once

#include "core/UUID.h"
#include "core/Timestep.h"
#include "renderer/Texture.h"

#include <unordered_map>
#include <filesystem>
#include <future>

namespace Omni
{
	class AssetManager
	{
	public:

		struct Texture2DProps
		{
			TextureFilteringMode MinFilter = TextureFilteringMode::Nearest;
			TextureFilteringMode MagFilter = TextureFilteringMode::Nearest;
			TextureWrapMode WrapMode = TextureWrapMode::Repeat;
			bool GenerateMips = true;
		};

		enum AssetType
		{
			AssetType_Misc,
			AssetType_Scene,
			AssetType_Shader,
			AssetType_Texture2D,
			AssetType_Font,
			AssetType_Text,
		};

		struct AssetFile
		{
			AssetType AssetType = AssetType_Misc;
			bool IsLoaded = false;
			std::string FilePath = "";
			uint64_t SecondsSinceLastNeeded = 0;
			long long MsSinceModified = 0;
		};

		static void Init();
		static void OnUpdate(Timestep ts);
		static void OnFixedUpdate();
		static void Refresh();
		static void Shutdown();

		static std::filesystem::path GetAssetPath() { return s_AssetPath; }
		static void SetAssetPath(const std::filesystem::path& assetPath) { s_AssetPath = assetPath; }
		static std::filesystem::path GetAssetMapPath() { return s_AssetMapPath; }
		static void SetAssetMapPath(const std::filesystem::path& assetMapPath) { s_AssetMapPath = assetMapPath; }

		static void SetRefreshInterval(uint64_t refreshInterval) { m_RefreshInterval = refreshInterval; }
		static uint64_t GetRefreshInterval() { return m_RefreshInterval; }

		static AssetFile GetAsset(const UUID& assetID);
		static AssetFile GetAsset(const std::string& filepath);
		static UUID GetAssetID(const std::string& filepath);
		static const std::string& GetAssetPath(const UUID& assetID);

		static bool IsAssetLoaded(const UUID& assetID);
		static bool IsAssetLoaded(const std::string& filepath);

		static bool DoesAssetExist(const UUID& assetID);
		static bool DoesAssetExist(const std::string& filepath);

		static bool CreateAssetFile(const std::string& filepath);
		static bool CreateFolder(const std::string& folder);
		static bool RenameFile(const UUID& assetID, const std::string& newName);
		static bool RenameFolder(std::filesystem::directory_entry folder, const std::string& newName);
		static bool DeleteAssetFile(const std::string& filepath);
		static bool DeleteFolder(const std::string& folder);

		static const std::string& GetTextFile(const UUID& assetID);
		static const std::string& LoadTextFile(const UUID& assetID);
		static void UnloadTextFile(const UUID& assetID);
		static bool WriteToTextFile(const std::string& filepath, const std::string& data);

		static Ref<Texture2D> GetTexture2D(const UUID& assetID);
		static Ref<Texture2D> LoadTexture2D(const UUID& assetID);
		static Ref<Texture2D> LoadTexture2DAsync(const UUID& assetID);
		static void UnloadTexture2D(const UUID& assetID);

		// Asset Props

		static bool DoesAssetHavePropsFile(const UUID& assetID);

		static void SaveTexture2DAssetProps(const UUID& assetID, Texture2DProps texture2DProps);
		static Texture2DProps LoadTexture2DAssetProps(const UUID& assetID);

		// Asset Props

		static std::string AssetTypeToString(AssetManager::AssetType assetType);
		static AssetManager::AssetType StringToAssetType(const std::string& assetTypeStr);

	private:

		static bool IsEntryNameValid(const std::string& name);
		static bool IsFileRegistered(std::filesystem::directory_entry filepath);

		static void SaveAssetMap();
		static bool LoadAssetMap();

		static void RegisterFile(std::filesystem::directory_entry filepath);
		static void UnregisterFile(UUID assetID);

		static void UnloadUnusedAssets();
		static long long GetMsSinceModified(const std::string& filepath);

		static void LoadTexture2DData(const UUID& assetID);

	public:

		inline static std::filesystem::path s_AssetPath = "assets";
		inline static std::filesystem::path s_AssetMapPath = "assetdata/assetdata.assetmap";

		inline static uint64_t m_FixedUpdateCount = 0;
		inline static uint64_t m_RefreshInterval = 60;

		inline static std::filesystem::path s_CurrentDirectory;
		inline static std::unordered_map<UUID, AssetFile> s_AssetMap;

		inline static std::unordered_map<UUID, Ref<std::string>> s_TextFiles;
		inline static std::unordered_map<UUID, Ref<Texture2D>> s_Texture2Ds;
		inline static std::unordered_map<UUID, Texture2DData*> s_Texture2DData;
	};
}
