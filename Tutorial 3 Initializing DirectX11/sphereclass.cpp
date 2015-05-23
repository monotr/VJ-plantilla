//////////////////////////
//Filename: modelclass.cpp
//////////////////////////
#include "sphereclass.h"

SphereClass::SphereClass(){
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Position = D3DXVECTOR3(0, 0, 0);
	m_Rotation = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(0, 0, 0);
}

SphereClass::SphereClass(const SphereClass& other){
}

SphereClass::~SphereClass(){
}

bool SphereClass::Initialize(ID3D11Device* device, D3DXVECTOR3 position, int stacks, int slides, float radius, D3DXVECTOR4 color){

	bool result;

	m_stackCount = (unsigned int) stacks;
	m_sliceCount = (unsigned int) slides;
	m_radius = (unsigned int) radius;


	if (position)
		m_Position = position;

	//Initialize the vertex and index buffer that hold the geometry for the triangle
	result = InitializeBuffers(device);
	if (!result)
		return false;

	return true;
}

void SphereClass::Shutdown(){

	//Release the vertex and index buffers;
	ShutdownBuffers();

	return;
}

void SphereClass::Render(ID3D11DeviceContext* deviceContext){

	//Put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	RenderBuffers(deviceContext);

	return;
}

int SphereClass::GetIndexCount(){

	return m_indexCount;
}

bool SphereClass::InitializeBuffers(ID3D11Device* device){

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int index = 0;
	const float phi = D3DX_PI * 2.0;
	const float theta = D3DX_PI;

	//Set the number of vertices in the vertex array
	m_vertexCount = (m_stackCount * m_sliceCount);

	//Set the number of indices in the index array
	m_indexCount = ((m_stackCount-1) * (m_sliceCount-1)*6);

	//Create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	//Create the index array
	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	
	//Load the vertex and index array with data
	for (int i = 0; i < m_stackCount; i++){
		float y = (float) i / (m_stackCount - 1);
		for (int j = 0; j < m_sliceCount; j++){
			float x = (float) j / (m_sliceCount - 1);

			vertices[index].position.x = m_radius * sin(y*theta) * cos(x*phi);
			vertices[index].position.y = m_radius * cos(y*theta);
			vertices[index].position.z = m_radius * sin(y*theta) * sin(x*phi);

			vertices[index].texture.x = x;
			vertices[index].texture.y = y;

			index++;
		}
	}

	index = 0;

	for (int i = 0; i < m_stackCount - 1; i++){
		for (int j = 0; j < m_sliceCount - 1; j++){
			int index1 = (m_sliceCount * i) + j;
			int index2 = (m_sliceCount * i) + (j + 1);
			int index3 = (m_sliceCount * (i + 1)) + j;
			int index4 = (m_sliceCount * (i + 1)) + (j + 1);

			indices[index++] = index1;
			indices[index++] = index3;
			indices[index++] = index4;

			indices[index++] = index1;
			indices[index++] = index4;
			indices[index++] = index2;
		}
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

void SphereClass::ShutdownBuffers(){

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

void SphereClass::RenderBuffers(ID3D11DeviceContext* deviceContext){

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

void SphereClass::SetPosition(D3DXVECTOR3 position){
	m_Position = position;
}

void SphereClass::SetRotation(D3DXVECTOR3 rotation){
	m_Rotation = rotation;
}

void SphereClass::SetScale(D3DXVECTOR3 scale){
	m_Scale = scale;
}

D3DXVECTOR3 SphereClass::GetPosition(){
	return m_Position;
}

D3DXVECTOR3 SphereClass::GetRotation(){
	return m_Rotation;
}

D3DXVECTOR3 SphereClass::GetScale(){
	return m_Scale;
}

D3DXMATRIX SphereClass::GetMatrix(){
	D3DXMATRIX FinalM, TranslationM, RotationxM, RotationyM, RotationzM, ScaleM;
	D3DXMatrixIdentity(&FinalM); //Crea una matrix de identidad.
	D3DXMatrixTranslation(&TranslationM, m_Position.x, m_Position.y, m_Position.z);
	D3DXMatrixRotationX(&RotationxM, D3DXToDegree(m_Rotation.x));
	D3DXMatrixRotationY(&RotationyM, D3DXToDegree(m_Rotation.y));
	D3DXMatrixRotationZ(&RotationzM, D3DXToDegree(m_Rotation.z));
	D3DXMatrixScaling(&ScaleM, m_Scale.x, m_Scale.y, m_Scale.z);
	FinalM = FinalM *  ScaleM *TranslationM *  RotationxM * RotationyM * RotationzM;
	return FinalM;
}