///////////////////////////
//Filename: sphereclass.h
///////////////////////////
#ifndef _SPHERECLASS_H_
#define _SPHERECLASS_H_

/////////////
//INCLUDES///
/////////////
#include <D3D11.h>
#include <D3DX10math.h>

/////////////////////
// Class name: ModelClass
/////////////////////
class SphereClass{
private:
	struct VertexType{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	SphereClass();
	SphereClass(const SphereClass&);
	~SphereClass();

	bool Initialize(ID3D11Device*, D3DXVECTOR3, int, int, float, D3DXVECTOR4);
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

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount, m_stackCount, m_sliceCount, m_radius;
	D3DXVECTOR3 m_Position, m_Rotation, m_Scale;

};

#endif