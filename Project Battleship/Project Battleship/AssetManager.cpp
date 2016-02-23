#include "AssetManager.h"
#include <iostream>
#include <fstream>
#include "WICTextureLoader.h"


CAssetManager::CAssetManager()
{
}


CAssetManager::~CAssetManager()
{
}


std::unique_ptr<TTexture> const& CAssetManager::getTexture(const std::string& name)
{
	auto it = texturesMap.find(name);
	if (it == texturesMap.end()) throw std::invalid_argument("entry not found");
	return it->second;
}

void CAssetManager::Init(ID3D11Device* d3d_Device, ID3D11DeviceContext* pDeviceContext)
{
	
	std::vector<std::string> pngFilePaths;
	FindFilesByPath("Assets/", ".png", pngFilePaths);
	for (size_t i = 0; i < pngFilePaths.size(); i++)
	{
		LoadTexturePNG(d3d_Device, pDeviceContext, pngFilePaths[i]);
	}
}

void CAssetManager::LoadTexturePNG(ID3D11Device* d3d_Device, ID3D11DeviceContext* pDeviceContext, std::string filePath)
{
	ID3D11ShaderResourceView* m_pTempTexture = nullptr;
	const std::wstring path_wstr(filePath.begin(), filePath.end());

	std::basic_ifstream<unsigned char> file(path_wstr, std::ios::binary);

	CreateWICTextureFromFile(d3d_Device, pDeviceContext, path_wstr.c_str(), NULL, &m_pTempTexture, NULL);

	std::string name;
	std::string delimiter = "/";
	size_t pos = 0;
	while ((pos = filePath.find(delimiter)) != std::string::npos)
	{
		name = filePath.substr(0, pos);
		filePath.erase(0, pos + delimiter.length());
	}
	delimiter = ".png";
	pos = 0;
	while ((pos = filePath.find(delimiter)) != std::string::npos)
	{
		name = filePath.substr(0, pos);
		filePath.erase(0, pos + delimiter.length());
	}
	texturesMap[name] = std::unique_ptr<TTexture>(new TTexture(m_pTempTexture, name));
}

void CAssetManager::FindFilesByPath(std::string folderPath, std::string fileExtension, std::vector<std::string>& filePaths)
{
	std::string search_path = folderPath + "*" + fileExtension;

	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::string path = folderPath + fd.cFileName;
				filePaths.push_back(path);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
}