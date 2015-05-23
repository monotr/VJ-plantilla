///////////////////////////
//Filename: cubeclass.h
///////////////////////////
#ifndef _CYLINDERCLASS_H_
#define _CYLINDERCLASS_H_

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
class CylinderClass {

private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};

	struct ModelType{

		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
	};

	struct TempVertexType{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType{
		float x, y, z;
	};


public:
	CylinderClass();
	CylinderClass(const CylinderClass&);
	~CylinderClass();

	bool Initialize(ID3D11Device*, WCHAR*, D3DXVECTOR3, int, int, float);
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

	ID3D11ShaderResourceView** GetTextureArray();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);
	void CalculateNormal(VectorType, VectorType, VectorType&);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount, m_Slices;
	float m_Radius, m_Height;
	TextureClass* m_Texture;
	D3DXVECTOR3 m_Position, m_Rotation, m_Scale;

	//TextureArrayClass* m_TextureArray;

	ModelType* m_cylinder;
};

#endif