#include "ompch.h"
#include "AssetManager.h"
#include <yaml-cpp/yaml.h>

namespace Omni
{
	static std::mutex s_AssetMapMutex;

	static std::mutex s_Texture2DMutex;
	static std::vector<std::future<void>> s_Texture2DDataFutures;

	void AssetManager::Init()
	{
		s_CurrentDirectory = s_AssetPath;

		LoadAssetMap();
		Refresh();
	}

	void AssetManager::OnUpdate(Timestep ts)
	{
		if (!s_Texture2DData.empty())
		{
			for (auto& texData : s_Texture2DData)
			{
				s_Texture2Ds[texData.first] = Texture2D::Generate(texData.second);
				delete texData.second;
			}
			s_Texture2DData.clear();
			s_Texture2DDataFutures.clear();
		}
	}

	void AssetManager::OnFixedUpdate()
	{
		if (m_FixedUpdateCount % m_RefreshInterval == 0)
		{
			Refresh();
		}

		if (m_FixedUpdateCount % 60 == 0)
		{
			UnloadUnusedAssets();
		}

		m_FixedUpdateCount++;
	}

	void AssetManager::Refresh()
	{
		bool needToSave = false;
		std::vector<std::filesystem::directory_entry> foundFiles;

		for (auto& directoryEntry : std::filesystem::recursive_directory_iterator(s_CurrentDirectory))
		{
			// Ignore folders and .assetprops files
			if (!directoryEntry.is_directory())
			{
				if (directoryEntry.path().extension() != ".assetprops")
				{
					foundFiles.push_back(directoryEntry);

					// If the file is not already registered, register it
					std::lock_guard<std::mutex> asssetMapLock(s_AssetMapMutex);
					if (!IsFileRegistered(directoryEntry))
					{
						needToSave = true;
						RegisterFile(directoryEntry);
					}
				}
			}
		}

		// Unregister the files that were not found
		std::vector<UUID> filesToUnregister;
		std::lock_guard<std::mutex> asssetMapLock(s_AssetMapMutex);
		for (auto& file : s_AssetMap)
		{
			if (std::find(foundFiles.cbegin(), foundFiles.cend(), file.second.FilePath) == foundFiles.cend())
			{
				filesToUnregister.push_back(file.first);
			}
		}

		for (auto& file : filesToUnregister)
		{
			needToSave = true;
			UnregisterFile(file);
		}

		if (needToSave)
			SaveAssetMap();

		for (auto& asset : s_AssetMap)
		{
			if (asset.second.IsLoaded)
			{
				long long msSinceModified = GetMsSinceModified(asset.second.FilePath);
				if (asset.second.MsSinceModified != msSinceModified)
				{
					asset.second.MsSinceModified = msSinceModified;

					switch (asset.second.AssetType)
					{
					case AssetType_Text:
						LoadTextFile(asset.first);
						break;

					case AssetType_Texture2D:
						UnloadTexture2D(asset.first);
						LoadTexture2D(asset.first);
						break;
					}
				}
			}
		}
	}

	void AssetManager::Shutdown()
	{
		SaveAssetMap();
	}

	bool AssetManager::IsEntryNameValid(const std::string& name)
	{
		if (name == "" || name == ".")
			return false;
		const std::string illegalChars = "<>:\"/\\|?*";
		for (size_t i = 0; i < illegalChars.length(); i++)
		{
			if (name.find(illegalChars[i]) != std::string::npos)
			{
				return false;
			}
		}
		return true;
	}

	bool AssetManager::IsFileRegistered(std::filesystem::directory_entry filepath)
	{
		for (auto& asset : s_AssetMap)
		{
			if (asset.second.FilePath == filepath)
			{
				return true;
			}
		}
		return false;
	}

	void AssetManager::SaveAssetMap()
	{
		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "Assets" << YAML::Value << YAML::BeginSeq;
		for (auto& asset : s_AssetMap)
		{
			out << YAML::BeginMap;

			out << YAML::Key << "ID" << YAML::Value << asset.first;
			out << YAML::Key << "Type" << YAML::Value << AssetTypeToString(asset.second.AssetType);
			out << YAML::Key << "Path" << YAML::Value << asset.second.FilePath;

			out << YAML::EndMap;
		}
		out << YAML::EndSeq;

		out << YAML::EndMap;

		std::ofstream fout(s_AssetMapPath);
		if (fout.is_open())
		{
			fout << out.c_str();
			fout.close();
		}
		else
		{
			OM_CORE_ERROR("Could not save asset map to \"{0}\"!", s_AssetMapPath.string());
		}
	}

	bool AssetManager::LoadAssetMap()
	{
		std::ifstream stream(s_AssetMapPath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Assets"])
			return false;

		auto assetData = data["Assets"];
		if (assetData)
		{
			for (auto asset : assetData)
			{
				s_AssetMap[asset["ID"].as<uint64_t>()] = AssetFile{
					StringToAssetType(asset["Type"].as<std::string>()),
					false,
					asset["Path"].as<std::string>()};
			}
		}
	}

	void AssetManager::RegisterFile(std::filesystem::directory_entry filepath)
	{
		std::string fileExtensionString = filepath.path().extension().string();
		UUID assetID = UUID();

		if (fileExtensionString == ".omscene")
		{
			s_AssetMap[assetID] = AssetFile{ AssetType::AssetType_Scene, false, filepath.path().string() };
		}
		else if (fileExtensionString == ".glsl" ||
				 fileExtensionString == ".vert" ||
				 fileExtensionString == ".frag" || 
				 fileExtensionString == ".pixel")
		{
			s_AssetMap[assetID] = AssetFile{ AssetType::AssetType_Shader, false, filepath.path().string() };

		}
		else if (fileExtensionString == ".png")
		{
			s_AssetMap[assetID] = AssetFile{ AssetType::AssetType_Texture2D, false, filepath.path().string() };
			SaveTexture2DAssetProps(assetID, Texture2DProps());
		}
		else if (fileExtensionString == ".ttf")
		{
			s_AssetMap[assetID] = AssetFile{ AssetType::AssetType_Font, false, filepath.path().string() };
		}
		else if (fileExtensionString == ".txt" ||
				 fileExtensionString == ".md" ||
				 fileExtensionString == ".json" ||
				 fileExtensionString == ".xml" ||
				 fileExtensionString == ".yaml")
		{
			s_AssetMap[assetID] = AssetFile{ AssetType::AssetType_Text, false, filepath.path().string() };
		}
		else
		{
			s_AssetMap[assetID] = AssetFile{ AssetType::AssetType_Misc, false, filepath.path().string() };
		}
	}

	// TODO : Consider if unregistered files need to be unloaded from their respective asset caches

	void AssetManager::UnregisterFile(UUID assetID)
	{
		s_AssetMap.erase(assetID);
	}

	void AssetManager::UnloadUnusedAssets()
	{
		for (auto& asset : s_AssetMap)
		{
			if (asset.second.IsLoaded)
			{
				switch (asset.second.AssetType)
				{
				case AssetType_Texture2D:
					if (asset.second.SecondsSinceLastNeeded >= 5)
					{
						asset.second.SecondsSinceLastNeeded = 0;
						UnloadTexture2D(asset.first);
					}
					break;
				case AssetType_Text:
					if (asset.second.SecondsSinceLastNeeded >= 1)
					{
						asset.second.SecondsSinceLastNeeded = 0;
						UnloadTextFile(asset.first);
					}
					break;
				}
				asset.second.SecondsSinceLastNeeded++;
			}
		}
	}

	long long AssetManager::GetMsSinceModified(const std::string& filepath)
	{
		return std::chrono::time_point_cast<std::chrono::milliseconds>(std::filesystem::directory_entry(filepath).last_write_time()).time_since_epoch().count();
	}

	AssetManager::AssetFile AssetManager::GetAsset(const UUID& assetID)
	{
		return s_AssetMap[assetID];
	}

	AssetManager::AssetFile AssetManager::GetAsset(const std::string& filepath)
	{
		return s_AssetMap[GetAssetID(filepath)];
	}

	UUID AssetManager::GetAssetID(const std::string& filepath)
	{
		for (auto& asset : s_AssetMap)
		{
			if (asset.second.FilePath == filepath)
			{
				return asset.first;
			}
		}
		return -1;
	}

	const std::string& AssetManager::GetAssetPath(const UUID& assetID)
	{
		return s_AssetMap[assetID].FilePath;
	}

	bool AssetManager::IsAssetLoaded(const UUID& assetID)
	{
		return s_AssetMap[assetID].IsLoaded;
	}

	bool AssetManager::IsAssetLoaded(const std::string& filepath)
	{
		return s_AssetMap[GetAssetID(filepath)].IsLoaded;
	}

	bool AssetManager::DoesAssetExist(const UUID& assetID)
	{
		if (s_AssetMap.find(assetID) != s_AssetMap.end())
		{
			return true;
		}
		return false;
	}

	bool AssetManager::DoesAssetExist(const std::string& filepath)
	{
		for (auto& asset : s_AssetMap)
		{
			if (asset.second.FilePath == filepath)
			{
				return true;
			}
		}
		return false;
	}

	bool AssetManager::CreateAssetFile(const std::string& filepath)
	{
		std::ofstream fout(filepath);
		if (fout.is_open())
		{
			fout.close();

			if (!DoesAssetExist(filepath))
			{
				RegisterFile(std::filesystem::directory_entry(filepath));
			}
			return true;
		}
		OM_CORE_WARN("[AssetManager::CreateAssetFile] Could not create file at \"{0}\"!", filepath);
		return false;
	}

	bool AssetManager::CreateFolder(const std::string& folder)
	{
		bool folderCreated = std::filesystem::create_directory(folder);
		if (folderCreated)
			return true;
		OM_CORE_ERROR("[AssetManager::CreateFolder] Could not create folder at \"{0}\"!", folder);
		return false;
	}

	bool AssetManager::RenameFile(const UUID& assetID, const std::string& newName)
	{
		std::filesystem::directory_entry oldFilePath = std::filesystem::directory_entry(GetAssetPath(assetID));
		if (!IsEntryNameValid(newName))
		{
			OM_CORE_WARN("[AssetManager::RenameFile] New file name \"{0}\" is invalid!", newName);
			return false;
		}

		std::filesystem::path newPath = oldFilePath.path().parent_path().append(newName);
		if (std::rename(oldFilePath.path().string().c_str(), newPath.string().c_str()))
		{
			OM_CORE_WARN("[AssetManager::RenameFile] Error renaming file \"{0}\" to \"{1}\"!", oldFilePath.path().string(), newPath.string());
			return false;
		}

		s_AssetMap[assetID].FilePath = newPath.string();
		SaveAssetMap();
		return true;
	}

	bool AssetManager::RenameFolder(std::filesystem::directory_entry folder, const std::string& newName)
	{
		if (!IsEntryNameValid(newName))
		{
			OM_CORE_WARN("[AssetManager::RenameFolder] New folder name \"{0}\" is invalid!", newName);
			return false;
		}

		std::filesystem::path renamedFolderPath = folder.path().parent_path().append(newName);
		if (std::rename(folder.path().string().c_str(), renamedFolderPath.string().c_str()))
		{
			OM_CORE_WARN("[AssetManager::RenameFolder] Error renaming file \"{0}\" to \"{1}\"!", folder.path().string(), renamedFolderPath.string());
			return false;
		}

		for (auto& asset : s_AssetMap)
		{
			std::string folderPathStr = folder.path().string() + "\\";
			if (folderPathStr.length() <= asset.second.FilePath.length())
			{
				bool pathFound = true;
				for (size_t i = 0; i < folderPathStr.length(); i++)
				{
					if (folderPathStr[i] != asset.second.FilePath[i])
					{
						pathFound = false;
						break;
					}
				}
				if (pathFound)
				{
					std::string trimmedAssetPath = asset.second.FilePath;
					trimmedAssetPath.erase(0, folderPathStr.length() - 1);
					asset.second.FilePath = renamedFolderPath.string() + trimmedAssetPath;
				}
			}
		}

		SaveAssetMap();
		return true;
	}

	bool AssetManager::DeleteAssetFile(const std::string& filepath)
	{
		bool fileDeleted = std::filesystem::remove(filepath);
		if (fileDeleted)
			UnregisterFile(GetAssetID(filepath));
		else
			OM_CORE_WARN("[AssetManager::DeleteAssetFile] Could not delete file \"{0}\"!", filepath);
		return fileDeleted;
	}

	bool AssetManager::DeleteFolder(const std::string& folder)
	{
		bool folderDeleted = std::filesystem::remove_all(folder);
		if (!folderDeleted)
			OM_CORE_WARN("[AssetManager::DeleteFolder] Could not delete folder \"{0}\"!", folder);
		return folderDeleted;
	}

	#pragma region Text File

	const std::string& AssetManager::GetTextFile(const UUID& assetID)
	{
		s_AssetMap[assetID].SecondsSinceLastNeeded = 0;
		return *s_TextFiles[assetID];
	}

	const std::string& AssetManager::LoadTextFile(const UUID& assetID)
	{
		s_AssetMap[assetID].SecondsSinceLastNeeded = 0;
		std::ifstream stream(s_AssetMap[assetID].FilePath);
		std::stringstream strStream;
		strStream << stream.rdbuf();
		std::string fileContent = strStream.str();

		s_TextFiles[assetID] = CreateRef<std::string>(fileContent);
		s_AssetMap[assetID].IsLoaded = true;
		s_AssetMap[assetID].MsSinceModified = GetMsSinceModified(s_AssetMap[assetID].FilePath);
		return fileContent;
	}

	void AssetManager::UnloadTextFile(const UUID& assetID)
	{
		s_TextFiles.erase(assetID);
		s_AssetMap[assetID].IsLoaded = false;
	}

	bool AssetManager::WriteToTextFile(const std::string& filepath, const std::string& data)
	{
		std::ofstream fout(filepath);
		if (fout.is_open())
		{
			fout << data.c_str();
			fout.close();

			if (!DoesAssetExist(filepath))
			{
				RegisterFile(std::filesystem::directory_entry(filepath));
			}
			return true;
		}
		OM_CORE_ERROR("[AssetManager::WriteToTextFile] Could not write to \"{0}\"!", filepath);
		return false;
	}
	
	#pragma endregion

	#pragma region Texture2D

	Ref<Texture2D> AssetManager::GetTexture2D(const UUID& assetID)
	{
		s_AssetMap[assetID].SecondsSinceLastNeeded = 0;
		return s_Texture2Ds[assetID];
	}

	void AssetManager::LoadTexture2DData(const UUID& assetID)
	{
		OM_PROFILE_FUNCTION();
		std::string filepath = s_AssetMap[assetID].FilePath;

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			OM_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		}

		if (!data)
		{
			OM_CORE_ERROR("[AssetManager::LoadTexture2DData] Could not load texture from path \"{0}\"!", filepath);
			return;
		}

		Texture2DData* texture2DData = new Texture2DData();
		texture2DData->Memory = data;
		texture2DData->Width = width;
		texture2DData->Height = height;
		texture2DData->Channels = channels;

		if (DoesAssetHavePropsFile(assetID))
		{
			Texture2DProps texture2DProps = LoadTexture2DAssetProps(assetID);
			texture2DData->MinFilter = texture2DProps.MinFilter;
			texture2DData->MagFilter = texture2DProps.MagFilter;
			texture2DData->WrapMode = texture2DProps.WrapMode;
			texture2DData->GenerateMips = texture2DProps.GenerateMips;
		}

		s_Texture2DData[assetID] = texture2DData;
	}

	Ref<Texture2D> AssetManager::LoadTexture2D(const UUID& assetID)
	{
		s_Texture2Ds[assetID] = Texture2D::Create(s_AssetMap[assetID].FilePath);
		s_AssetMap[assetID].IsLoaded = true;
		s_AssetMap[assetID].SecondsSinceLastNeeded = 0;
		s_AssetMap[assetID].MsSinceModified = GetMsSinceModified(s_AssetMap[assetID].FilePath);
		return s_Texture2Ds[assetID];
	}

	Ref<Texture2D> AssetManager::LoadTexture2DAsync(const UUID& assetID)
	{
		Ref<Texture2D> whiteTex = Texture2D::Create(TextureSpecification());
		uint32_t whiteTextureData = 0xffffffff;
		whiteTex->SetData(&whiteTextureData, sizeof(uint32_t));

		std::lock_guard<std::mutex> texture2DLock(s_Texture2DMutex);
		s_Texture2Ds[assetID] = whiteTex;
		s_Texture2DDataFutures.push_back(std::async(std::launch::async, LoadTexture2DData, assetID));

		s_AssetMap[assetID].IsLoaded = true;
		s_AssetMap[assetID].SecondsSinceLastNeeded = 0;
		s_AssetMap[assetID].MsSinceModified = GetMsSinceModified(s_AssetMap[assetID].FilePath);

		return s_Texture2Ds[assetID];
	}

	void AssetManager::UnloadTexture2D(const UUID& assetID)
	{
		s_Texture2Ds.erase(assetID);
		s_AssetMap[assetID].IsLoaded = false;
	}

	#pragma endregion

	bool AssetManager::DoesAssetHavePropsFile(const UUID& assetID)
	{
		auto entry = std::filesystem::directory_entry(s_AssetMap[assetID].FilePath + ".assetprops");
		if (entry.exists())
			return true;
		else
			return false;
	}

	void AssetManager::SaveTexture2DAssetProps(const UUID& assetID, Texture2DProps texture2DProps)
	{
		std::string assetPropsPath = GetAssetPath(assetID) + ".assetprops";

		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "ID" << YAML::Value << assetID;
		out << YAML::Key << "Type" << YAML::Value << AssetTypeToString(s_AssetMap[assetID].AssetType);

		if (texture2DProps.MinFilter == TextureFilteringMode::Nearest)
			out << YAML::Key << "Min Filter" << YAML::Value << "Nearest";
		else if (texture2DProps.MinFilter == TextureFilteringMode::Linear)
			out << YAML::Key << "Min Filter" << YAML::Value << "Linear";

		if (texture2DProps.MagFilter == TextureFilteringMode::Nearest)
			out << YAML::Key << "Mag Filter" << YAML::Value << "Nearest";
		else if (texture2DProps.MagFilter == TextureFilteringMode::Linear)
			out << YAML::Key << "Mag Filter" << YAML::Value << "Linear";

		if (texture2DProps.WrapMode == TextureWrapMode::Repeat)
			out << YAML::Key << "Wrap Mode" << YAML::Value << "Repeat";
		else if (texture2DProps.WrapMode == TextureWrapMode::Clamp)
			out << YAML::Key << "Wrap Mode" << YAML::Value << "Clamp";

		out << YAML::Key << "Generate MipMaps" << YAML::Value << texture2DProps.GenerateMips;

		out << YAML::EndMap;

		std::ofstream fout(assetPropsPath);
		if (fout.is_open())
		{
			fout << out.c_str();
			fout.close();
		}
		else
		{
			OM_CORE_ERROR("Could not save asset properties to \"{0}\"!", assetPropsPath);
		}
	}

	AssetManager::Texture2DProps AssetManager::LoadTexture2DAssetProps(const UUID& assetID)
	{
		if (DoesAssetHavePropsFile(assetID))
		{
			AssetFile& asset = s_AssetMap[assetID];
			std::string assetPropsPath = asset.FilePath + ".assetprops";

			std::ifstream stream(assetPropsPath);
			std::stringstream strStream;
			strStream << stream.rdbuf();

			YAML::Node data = YAML::Load(strStream.str());

			if (data)
			{
				Texture2DProps texture2DProps = Texture2DProps();

				std::string minFilterStr = data["Min Filter"].as<std::string>();
				if (minFilterStr == "Nearest")
					texture2DProps.MinFilter = TextureFilteringMode::Nearest;
				else if (minFilterStr == "Linear")
					texture2DProps.MinFilter = TextureFilteringMode::Linear;

				std::string magFilterStr = data["Mag Filter"].as<std::string>();
				if (magFilterStr == "Nearest")
					texture2DProps.MagFilter = TextureFilteringMode::Nearest;
				else if (magFilterStr == "Linear")
					texture2DProps.MagFilter = TextureFilteringMode::Linear;

				std::string wrapModeStr = data["Wrap Mode"].as<std::string>();
				if (wrapModeStr == "Repeat")
					texture2DProps.WrapMode = TextureWrapMode::Repeat;
				else if (wrapModeStr == "Clamp")
					texture2DProps.WrapMode = TextureWrapMode::Clamp;

				texture2DProps.GenerateMips = data["Generate MipMaps"].as<bool>();
				return texture2DProps;
			}
			return Texture2DProps();
		}
	}

	std::string AssetManager::AssetTypeToString(AssetManager::AssetType assetType)
	{
		switch (assetType)
		{
		case AssetManager::AssetType_Misc:
			return "Misc";
			break;
		case AssetManager::AssetType_Scene:
			return "Scene";
			break;
		case AssetManager::AssetType_Shader:
			return "Shader";
			break;
		case AssetManager::AssetType_Texture2D:
			return "Texture2D";
			break;
		case AssetManager::AssetType_Font:
			return "Font";
			break;
		case AssetManager::AssetType_Text:
			return "Text";
			break;
		default:
			OM_CORE_ASSERT(false, "[AssetTypeToString] Invalid Asset Type!");
			break;
		}
	}

	AssetManager::AssetType AssetManager::StringToAssetType(const std::string& assetTypeStr)
	{
		if (assetTypeStr == "Misc")
		{
			return AssetManager::AssetType_Misc;
		}
		else if (assetTypeStr == "Scene")
		{
			return AssetManager::AssetType_Scene;
		}
		else if (assetTypeStr == "Shader")
		{
			return AssetManager::AssetType_Shader;
		}
		else if (assetTypeStr == "Texture2D")
		{
			return AssetManager::AssetType_Texture2D;
		}
		else if (assetTypeStr == "Font")
		{
			return AssetManager::AssetType_Font;
		}
		else if (assetTypeStr == "Text")
		{
			return AssetManager::AssetType_Text;
		}
		else
		{
			OM_CORE_ERROR("[StringToAssetType] Could not determine asset type from string \"{0}\"!");
			return AssetManager::AssetType_Misc;
		}
	}
}
