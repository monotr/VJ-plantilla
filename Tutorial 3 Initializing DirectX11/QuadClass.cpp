//////////////////////////
//Filename: modelclass.cpp
//////////////////////////
#include "QuadClass.h"

QuadClass::QuadClass(){
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Position = D3DXVECTOR3(0, 0, 0);
	m_Rotation = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(0, 0, 0);
}

QuadClass::QuadClass(const QuadClass& other){
}

QuadClass::~QuadClass(){
}

bool QuadClass::Initialize(ID3D11Device* device, float u1, float v1){

	bool result;

	v = v1;
	u = u1;

	//Initialize the vertex and index buffer that hold the geometry for the triangle
	result = InitializeBuffers(device);
	if (!result)
		return false;

	return true;
}

void QuadClass::Shutdown(){

	//Release the vertex and index buffers;
	ShutdownBuffers();

	return;
}

void QuadClass::Render(ID3D11DeviceContext* deviceContext){

	//Put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	RenderBuffers(deviceContext);

	return;
}

int QuadClass::GetIndexCount(){

	return m_indexCount;
}

bool QuadClass::InitializeBuffers(ID3D11Device* device){

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//Set the number of vertices in the vertex array
	m_vertexCount = 5;

	//Set the number of indices in the index array
	m_indexCount = 6;

	//Create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	//Create the index array
	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	// Front
	vertices[0].position = D3DXVECTOR3(-1, 1, 0);
	vertices[1].position = D3DXVECTOR3(1, 1, 0);
	vertices[2].position = D3DXVECTOR3(-1, -1, 0);
	vertices[3].position = D3DXVECTOR3(1, -1, 0);
	/*vertices[0].texture = D3DXVECTOR2(u, u);
	vertices[1].texture = D3DXVECTOR2(v, u);
	vertices[2].texture = D3DXVECTOR2(u, v);
	vertices[3].texture = D3DXVECTOR2(v, v);*/
	vertices[0].texture = D3DXVECTOR2(0, 0);
	vertices[1].texture = D3DXVECTOR2(2, 0);
	vertices[2].texture = D3DXVECTOR2(0, 3);
	vertices[3].texture = D3DXVECTOR2(2, 3);


	//Front
	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 1;

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

void QuadClass::ShutdownBuffers(){

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

void QuadClass::RenderBuffers(ID3D11DeviceContext* deviceContext){

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

	return;
}

void QuadClass::SetPosition(D3DXVECTOR3 position){
	m_Position = position;
}

void QuadClass::SetRotation(D3DXVECTOR3 rotation){
	m_Rotation = rotation;
}

void QuadClass::SetScale(D3DXVECTOR3 scale){
	m_Scale = scale;
}

D3DXVECTOR3 QuadClass::GetPosition(){
	return m_Position;
}

D3DXVECTOR3 QuadClass::GetRotation(){
	return m_Rotation;
}

D3DXVECTOR3 QuadClass::GetScale(){
	return m_Scale;
}

D3DXMATRIX QuadClass::GetMatrix(){
	D3DXMATRIX FinalM, TranslationM, RotationxM, RotationyM, RotationzM, ScaleM;
	D3DXMatrixIdentity(&FinalM); //Crea una matrix de identidad.
	D3DXMatrixTranslation(&TranslationM, m_Position.x, m_Position.y, m_Position.z);
	D3DXMatrixRotationX(&RotationxM, D3DXToDegree(m_Rotation.x));
	D3DXMatrixRotationY(&RotationyM, D3DXToDegree(m_Rotation.y));
	D3DXMatrixRotationZ(&RotationzM, D3DXToDegree(m_Rotation.z));
	D3DXMatrixScaling(&ScaleM, m_Scale.x, m_Scale.y, m_Scale.z);
	FinalM = FinalM *  ScaleM * TranslationM *  RotationxM * RotationyM * RotationzM;
	/*FinalM = FinalM *  ScaleM * TranslationM *  RotationxM * RotationyM * RotationzM;*/
	return FinalM;
}