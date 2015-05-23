//////////////////////////
//Filename: modelclass.cpp
//////////////////////////
#include "coneclass.h"

ConeClass::ConeClass(){
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_Position = D3DXVECTOR3(0, 0, 0);
	m_Rotation = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(1, 1, 1);
	m_Slices = 0;
	m_Height = 0;
	m_Radius = 0;
}

ConeClass::ConeClass(const ConeClass& other){
}

ConeClass::~ConeClass(){
}

bool ConeClass::Initialize(ID3D11Device* device, WCHAR* textureFilename, D3DXVECTOR3 position, int height, int slices, float radius)
{
	bool result;

	m_Radius = radius;
	m_Height = height;
	m_Slices = slices;

	if (position)
		m_Position = position;

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device);
	if (!result)
		return false;

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
		return false;

	return true;
}

void ConeClass::Shutdown(){

	//Release the model texture
	ReleaseTexture();

	//Release the vertex and index buffers;
	ShutdownBuffers();

	return;
}

void ConeClass::Render(ID3D11DeviceContext* deviceContext){

	//Put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	RenderBuffers(deviceContext);

	return;
}

int ConeClass::GetIndexCount(){

	return m_indexCount;
}

ID3D11ShaderResourceView* ConeClass::GetTexture(){
	return m_Texture->GetTexture();
}

bool ConeClass::InitializeBuffers(ID3D11Device* device){

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int index = 0;
	int indexbase = 0;
	int indextop = 0;

	const float phix = D3DX_PI * 2.0;

	//Set the number of vertices in the vertex array
	m_vertexCount = m_Slices * 3;

	//Set the number of indices in the index array
	m_indexCount = m_Slices * 4 * 6;

	//Create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	//Create the index array
	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	//Load the vertex array with data

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < m_Slices; j++)
		{
			float x = (float)j / (m_Slices - 1);

			switch (i)
			{
			case 0:
				vertices[index].position.x = m_Radius * cos(x*phix);
				vertices[index].position.y = -m_Height;
				vertices[index].position.z = m_Radius * sin(x*phix);

				vertices[index].texture.y = 0;
				indexbase = index;
				break;

			case 1:
				vertices[index].position.x = 0;
				vertices[index].position.y = -m_Height;
				vertices[index].position.z = 0;

				vertices[index].texture.y = 1;
				indextop = index;
				break;

			case 2:
				vertices[index].position.x = 0;
				vertices[index].position.y = m_Height;
				vertices[index].position.z = 0;

				vertices[index].texture.y = 1;
				break;

			}

			vertices[index].texture.x = x;

			index++;
		}
	}

	//Load the index array with data

	index = 0;
	int newindex = 0;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < m_Slices - 1; j++)
		{
			switch (i)
			{
			case 0:
				indices[index++] = newindex; //314
				indices[index++] = newindex + 1;
				indices[index++] = indexbase + 1;
				break;
			case 1:
				indices[index++] = indextop + 1; //314
				indices[index++] = newindex + 1;
				indices[index++] = newindex;


				break;
			}
			newindex++;
		}
		newindex = 0;
	}

	//Set up the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Now create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
		return false;

	//Set up the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
		return false;

	//Release the arrays now that the vertex and index buffers have been created and loaded
	delete[]vertices;
	vertices = 0;

	delete[]indices;
	indices = 0;

	return true;

}

void ConeClass::ShutdownBuffers(){

	//Release the index buffer
	if (m_indexBuffer){
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	//Release the vertex buffer
	if (m_vertexBuffer){
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void ConeClass::RenderBuffers(ID3D11DeviceContext* deviceContext){

	unsigned int stride;
	unsigned int offset;

	//Set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	//Set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//Set the index buffer to active in th input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set the tupe of primitive that should be rendered from this vertex buffer, in this case triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	return;
}

bool ConeClass::LoadTexture(ID3D11Device* device, WCHAR* filename){

	bool result;

	//Create the texture object
	m_Texture = new TextureClass;
	if (!m_Texture)
		return false;

	//Initialize the texture object
	result = m_Texture->Initialize(device, filename);
	if (!result)
		return false;

	return true;
}

void ConeClass::ReleaseTexture(){

	if (m_Texture){
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

void ConeClass::SetPosition(D3DXVECTOR3 position){
	m_Position = position;
}
void ConeClass::SetRotation(D3DXVECTOR3 rotation){
	m_Rotation = rotation;
}
void ConeClass::SetScale(D3DXVECTOR3 scale){
	m_Scale = scale;
}
D3DXVECTOR3 ConeClass::GetPosition(){
	return m_Position;
}
D3DXVECTOR3 ConeClass::GetRotation(){
	return m_Rotation;
}
D3DXVECTOR3 ConeClass::GetScale(){
	return m_Scale;
}
D3DXMATRIX ConeClass::GetMatrix(){
	D3DXMATRIX FinalM, TranslationM, RotationxM, RotationyM, RotationzM, ScaleM;
	D3DXMatrixIdentity(&FinalM);
	D3DXMatrixTranslation(&TranslationM, m_Position.x, m_Position.y, m_Position.z);
	D3DXMatrixRotationX(&RotationxM, D3DXToDegree(m_Rotation.x));
	D3DXMatrixRotationY(&RotationyM, D3DXToDegree(m_Rotation.y));
	D3DXMatrixRotationZ(&RotationzM, D3DXToDegree(m_Rotation.z));
	D3DXMatrixScaling(&ScaleM, m_Scale.x, m_Scale.y, m_Scale.z);
	FinalM = FinalM  * ScaleM  * RotationxM * RotationyM * RotationzM * TranslationM;
	return FinalM;
}
