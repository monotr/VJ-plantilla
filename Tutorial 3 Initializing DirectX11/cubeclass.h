///////////////////////////
//Filename: cubeclass.h
///////////////////////////
#ifndef _CUBECLASS_H_
#define _CUBECLASS_H_

/////////////
//INCLUDES///
/////////////
#include <D3D11.h>
#include <D3DX10math.h>

/////////////////////
//MY CLASS INCLUDES//
/////////////////////
#include "textureclass.h"

/////////////////////
// Class name: ModelClass
/////////////////////
class CubeClass {

private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};


public:
	CubeClass();
	CubeClass(const CubeClass&);
	~CubeClass();

	bool Initialize(ID3D11Device*, WCHAR*, D3DXVECTOR3);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	D3DXVECTOR3 GetScale();
	D3DXMATRIX GetMatrix();

	void SetPosition(D3DXVECTOR3);
	void SetRotation(D3DXVECTOR3);
	void SetScale(D3DXVECTOR3);
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	D3DXVECTOR3 m_Position, m_Rotation, m_Scale;
};

#endif