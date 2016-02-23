#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include "Renderer.h"

struct TTexture
{
	ID3D11ShaderResourceView* m_pTexture;
	std::string id;
	TTexture();

	TTexture(ID3D11ShaderResourceView* tex, std::string i)
	{
		id = i;
		m_pTexture = tex;
	}
	~TTexture() { m_pTexture->Release(); }
};

class CAssetManager
{
public:
	CAssetManager();
	~CAssetManager();
	void Init(ID3D11Device* d3d_Device, ID3D11DeviceContext* pDeviceContext);
	void FindFilesByPath(std::string folderPath, std::string fileExtension, std::vector<std::string>& filePaths);

	std::unique_ptr<TTexture> const& getTexture(const std::string& name);
	void LoadTexturePNG(ID3D11Device* d3d_Device, ID3D11DeviceContext* pDeviceContext, std::string filePath);

private:
	std::map<std::string, std::unique_ptr<TTexture>> texturesMap;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

};

