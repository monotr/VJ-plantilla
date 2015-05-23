//////////////////////////
//Filename: modelclass.cpp
//////////////////////////
#include "cylinderclass.h"

CylinderClass::CylinderClass(){
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_Position = D3DXVECTOR3(0, 0, 0);
	m_Rotation = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(1, 1, 1);
	m_Slices = 0;
	m_Height = 0;
	m_Radius = 0;

	m_cylinder = 0;
}

CylinderClass::CylinderClass(const CylinderClass& other){
}

CylinderClass::~CylinderClass(){
}

bool CylinderClass::Initialize(ID3D11Device* device, WCHAR* textureFilename, D3DXVECTOR3 position, int height, int slices, float radius)
{
	bool result;

	m_Radius = radius;
	m_Height = height;
	m_Slices = slices;

	if (position)
		m_Position = position;

	//Calculate the normal,tangent, binormal, vectors for the model
	CalculateModelVectors();

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

void CylinderClass::Shutdown(){

	//Release the model texture
	ReleaseTexture();

	//Release the vertex and index buffers;
	ShutdownBuffers();

	return;
}



void CylinderClass::Render(ID3D11DeviceContext* deviceContext){

	//Put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	RenderBuffers(deviceContext);

	return;
}

int CylinderClass::GetIndexCount(){

	return m_indexCount;
}

ID3D11ShaderResourceView* CylinderClass::GetTexture(){
	return m_Texture->GetTexture();
}

bool CylinderClass::InitializeBuffers(ID3D11Device* device){

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
	m_vertexCount = m_Slices * 4;

	//Set the number of indices in the index array
	m_indexCount = m_Slices * 3 * 6;

	//Create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	//Create the index array
	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	//Load the vertex array with data

	for (int i = 0; i < 4; i++)
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
				vertices[index].position.x = m_Radius * cos(x*phix);
				vertices[index].position.y = m_Height;
				vertices[index].position.z = m_Radius * sin(x*phix);

				vertices[index].texture.y = 1;
				indextop = index;
				break;

			case 2:
				vertices[index].position.x = 0;
				vertices[index].position.y = -m_Height;
				vertices[index].position.z = 0;

				vertices[index].texture.y = 1;
				break;

			case 3:
				vertices[index].position.x = 0;
				vertices[index].position.y = m_Height;
				vertices[index].position.z = 0;

				vertices[index].texture.y = 0;
				break;
			}

			vertices[index].texture.x = x;

			index++;
		}
	}

	//Load the index array with data

	index = 0;
	int newindex = 0;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < m_Slices - 1; j++)
		{
			int index1 = (m_Slices *   i) + j;
			int index2 = (m_Slices *   i) + (j + 1);
			int index3 = (m_Slices * (i + 1)) + j;
			int index4 = (m_Slices * (i + 1)) + (j + 1);

			switch (i)
			{
			case 0:
				indices[index++] = index1; //314
				indices[index++] = index3;
				indices[index++] = index4;

				indices[index++] = index1; //124
				indices[index++] = index4;
				indices[index++] = index2;
				break;
			case 1:
				indices[index++] = newindex; //314
				indices[index++] = (newindex + 1);
				indices[index++] = indexbase;

				newindex++;
				break;
			case 2:
				indices[index++] = indextop; //314
				indices[index++] = (newindex + 1);
				indices[index++] = newindex;

				newindex++;
				break;
			}
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

void CylinderClass::ShutdownBuffers(){

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

void CylinderClass::RenderBuffers(ID3D11DeviceContext* deviceContext){

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

bool CylinderClass::LoadTexture(ID3D11Device* device, WCHAR* filename){

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

void CylinderClass::ReleaseTexture(){

	if (m_Texture){
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

void CylinderClass::SetPosition(D3DXVECTOR3 position){
	m_Position = position;
}
void CylinderClass::SetRotation(D3DXVECTOR3 rotation){
	m_Rotation = rotation;
}
void CylinderClass::SetScale(D3DXVECTOR3 scale){
	m_Scale = scale;
}
D3DXVECTOR3 CylinderClass::GetPosition(){
	return m_Position;
}
D3DXVECTOR3 CylinderClass::GetRotation(){
	return m_Rotation;
}
D3DXVECTOR3 CylinderClass::GetScale(){
	return m_Scale;
}
D3DXMATRIX CylinderClass::GetMatrix(){
	D3DXMATRIX FinalM, TranslationM, RotationxM, RotationyM, RotationzM, ScaleM;
	D3DXMatrixIdentity(&FinalM);
	D3DXMatrixTranslation(&TranslationM, m_Position.x, m_Position.y, m_Position.z);
	D3DXMatrixRotationX(&RotationxM, D3DXToDegree(m_Rotation.x));
	D3DXMatrixRotationY(&RotationyM, D3DXToDegree(m_Rotation.y));
	D3DXMatrixRotationZ(&RotationzM, D3DXToDegree(m_Rotation.z));
	D3DXMatrixScaling(&ScaleM, m_Scale.x, m_Scale.y, m_Scale.z);
	FinalM = FinalM  * ScaleM * TranslationM * RotationxM * RotationyM * RotationzM;
	return FinalM;
}

void CylinderClass::CalculateModelVectors(){

	int faceCount, i, index;
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal, normal;


	// Calculate the number of faces in the model.
	faceCount = m_vertexCount / 3;

	// Initialize the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (i = 0; i<faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1.x = m_cylinder[index].x;
		vertex1.y = m_cylinder[index].y;
		vertex1.z = m_cylinder[index].z;
		vertex1.tu = m_cylinder[index].tu;
		vertex1.tv = m_cylinder[index].tv;
		vertex1.nx = m_cylinder[index].nx;
		vertex1.ny = m_cylinder[index].ny;
		vertex1.nz = m_cylinder[index].nz;
		index++;

		vertex2.x = m_cylinder[index].x;
		vertex2.y = m_cylinder[index].y;
		vertex2.z = m_cylinder[index].z;
		vertex2.tu = m_cylinder[index].tu;
		vertex2.tv = m_cylinder[index].tv;
		vertex2.nx = m_cylinder[index].nx;
		vertex2.ny = m_cylinder[index].ny;
		vertex2.nz = m_cylinder[index].nz;
		index++;

		vertex3.x = m_cylinder[index].x;
		vertex3.y = m_cylinder[index].y;
		vertex3.z = m_cylinder[index].z;
		vertex3.tu = m_cylinder[index].tu;
		vertex3.tv = m_cylinder[index].tv;
		vertex3.nx = m_cylinder[index].nx;
		vertex3.ny = m_cylinder[index].ny;
		vertex3.nz = m_cylinder[index].nz;
		index++;

		// Calculate the tangent and binormal of that face.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Calculate the new normal using the tangent and binormal.
		CalculateNormal(tangent, binormal, normal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		m_cylinder[index - 1].nx = normal.x;
		m_cylinder[index - 1].ny = normal.y;
		m_cylinder[index - 1].nz = normal.z;
		m_cylinder[index - 1].tx = tangent.x;
		m_cylinder[index - 1].ty = tangent.y;
		m_cylinder[index - 1].tz = tangent.z;
		m_cylinder[index - 1].bx = binormal.x;
		m_cylinder[index - 1].by = binormal.y;
		m_cylinder[index - 1].bz = binormal.z;

		m_cylinder[index - 2].nx = normal.x;
		m_cylinder[index - 2].ny = normal.y;
		m_cylinder[index - 2].nz = normal.z;
		m_cylinder[index - 2].tx = tangent.x;
		m_cylinder[index - 2].ty = tangent.y;
		m_cylinder[index - 2].tz = tangent.z;
		m_cylinder[index - 2].bx = binormal.x;
		m_cylinder[index - 2].by = binormal.y;
		m_cylinder[index - 2].bz = binormal.z;

		m_cylinder[index - 3].nx = normal.x;
		m_cylinder[index - 3].ny = normal.y;
		m_cylinder[index - 3].nz = normal.z;
		m_cylinder[index - 3].tx = tangent.x;
		m_cylinder[index - 3].ty = tangent.y;
		m_cylinder[index - 3].tz = tangent.z;
		m_cylinder[index - 3].bx = binormal.x;
		m_cylinder[index - 3].by = binormal.y;
		m_cylinder[index - 3].bz = binormal.z;
	}
	return;
}

void CylinderClass::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3,
	VectorType& tangent, VectorType& binormal){

	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;

}

void CylinderClass::CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal){

	float length;


	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

	return;


}
