/////////////////////////////////////
//Filename: graphicsclass.cpp
/////////////////////////////////////
#include "graphicsclass.h"

float rotation;

GraphicsClass::GraphicsClass(){
	m_D3D = 0;
	m_Camera = 0;
	m_Sphere = 0;
	m_Cube = 0;
	m_Quad = 0;
	m_ColorShader = 0;
	m_Texture1 = 0;
	m_Texture2 = 0;
	m_Texture3 = 0;
	m_Texture4 = 0;
	m_TextureShader = 0;
	m_TextureShader2 = 0;
	m_TextureShader3 = 0;

	m_BumpMapShader = 0;

	m_LightShader = 0;
	m_Light = 0;
	m_Cone = 0;
	m_Cylinder = 0;
	m_Torus = 0;

	rotaCono = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other){
}

GraphicsClass::~GraphicsClass(){
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd){

	bool result;


	//Create the Direct3D Object
	m_D3D = new D3DClass;
	if(!m_D3D)
		return false;

	//Initialize the Direct3D object
	result = m_D3D->Initialize(screenWidth,screenHeight,VSYNC_ENABLED,hwnd,FULL_SCREEN,SCREEN_DEPTH,SCREEN_NEAR);
	if(!result){

		MessageBox(hwnd,L"Could not initialize Direct3D",L"Error",MB_OK);
	
	}

	//Create the camera object
	m_Camera = new cameraclass;
	if(!m_Camera)
		return false;

	

	//Set the initial position of the camera
	m_Camera->SetPosition(0.0f,20.0f,-30.0f);
	m_Camera->SetRotation(35.0f, 0.0f, 0.0f);
	//Create the sphere object
	m_Sphere = new SphereClass;
	if (!m_Sphere)
		return false;

	//Create the cube class
	m_Cube = new CubeClass;
	if (!m_Cube)
		return false;

	//Initialize the cone object
	result = m_Cube->Initialize(m_D3D->GetDevice(), L"seafloor.dds", D3DXVECTOR3(0, 0, 0));
	if (!result){
		MessageBox(hwnd, L"Could not initialize the Cube object.", L"Error", MB_OK);
		return false;
	}

	//Create the cone object
	m_Cone = new ConeClass;
	if (!m_Cone)
		return false;

	//Initialize the cone object
	result = m_Cone->Initialize(m_D3D->GetDevice(), L"seafloor.dds", D3DXVECTOR3(2, 0, 0), 2, 20, 2);
	if (!result){
		MessageBox(hwnd, L"Could not initialize the cone object.", L"Error", MB_OK);
		return false;
	}

	//Create the cylinder object
	m_Cylinder = new CylinderClass;
	if (!m_Cylinder)
		return false;

	//Initialize the cylinder object
	result = m_Cylinder->Initialize(m_D3D->GetDevice(), L"seafloor.dds", D3DXVECTOR3(-2, 0, 0), 2, 20, 2);
	if (!result){
		MessageBox(hwnd, L"Could not initialize the cylinder object.", L"Error", MB_OK);
		return false;
	}

	//Create the torus object
	m_Torus = new TorusClass;
	if (!m_Cylinder)
		return false;

	//Initialize the torus object
	result = m_Torus->Initialize(m_D3D->GetDevice(), L"seafloor.dds", D3DXVECTOR3(0, 2, -3), 20, 20, 5, 1);
	if (!result){
		MessageBox(hwnd, L"Could not initialize the torus object.", L"Error", MB_OK);
		return false;
	}
	
	//Create the light shader object
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
		return false;

	//Initialize the lightShaer object
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
		return false;

	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
		return false;

	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initiazlize the texture shader object.", L"Error", MB_OK);
	}

	m_TextureShader2 = new TextureShaderClass;
	if (!m_TextureShader2)
		return false;

	result = m_TextureShader2->Initialize2(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initiazlize the texture shader object.", L"Error", MB_OK);
	}

	m_TextureShader3 = new TextureShaderClass;
	if (!m_TextureShader3)
		return false;

	result = m_TextureShader3->Initialize3(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initiazlize the texture shader object.", L"Error", MB_OK);
	}

	m_Texture1 = new TextureClass;
	if(!m_Texture1)
		return false;

	result = m_Texture1->Initialize(m_D3D->GetDevice(), L"poste.jpg");

	m_Texture2 = new TextureClass;
	if (!m_Texture2)
		return false;

	result = m_Texture2->Initialize(m_D3D->GetDevice(), L"anuncio.jpg");

	m_Texture3 = new TextureClass;
	if (!m_Texture3)
		return false;

	result = m_Texture3->Initialize(m_D3D->GetDevice(), L"floor.jpg");

	m_Texture4 = new TextureClass;
	if (!m_Texture4)
		return false;

	result = m_Texture4->Initialize(m_D3D->GetDevice(), L"sphere.jpg");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture object.", L"Error", MB_OK);
		return false;
	}

	int stacks = 36;
	int slices = 36;
	float radius = 1;
	D3DXVECTOR3 positionSphere = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR4 colorSphere = D3DXVECTOR4(1, 1, 0, 1);
	result = m_Sphere->Initialize(m_D3D->GetDevice(), positionSphere, stacks, slices, radius, colorSphere);
	if (!result){
		MessageBox(hwnd, L"Could not initialize the sphere object.", L"Error", MB_OK);
		return false;
	}

	///////////////////////////////BUMP OBJ/////////////
	//Create the model object
	m_Model = new ModelClass;
	if (!m_Model)
		return false;

	//Initialize the textureshader object
	m_BumpMapShader = new BumpMapShaderClass;
	result = m_BumpMapShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result){
		MessageBox(hwnd, L"Could not initialize the BumpMapShader object.", L"Error", MB_OK);
		return false;
	}

	//Initialize the model object
	result = m_Model->Initialize(m_D3D->GetDevice(), "Cube.txt"
		, L"stone01.gif", L"bump01.gif");
	if (!result){
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	///////////////////////////////FIN BUMP OBJ/////////////

	////Create the color shader object
	//m_ColorShader = new ColorShaderClass;
	//if(!m_ColorShader)
	//	return false;

	////Initialize the color shader object
	//result = m_ColorShader->Initialize(m_D3D->GetDevice(),hwnd);
	//if(!result){
	//	MessageBox(hwnd,L"Could not initialize the color shader object.", L"Error",MB_OK);
	//	return false;
	//}

	m_Quad = new QuadClass;
	m_Quad->Initialize(m_D3D->GetDevice(), 0, 4);

	//Create the light object
	m_Light = new LightClass;
	if (!m_Light)
		return false;

	//Initialize the light object
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	return  true;
}

void GraphicsClass::Shutdown(){

	// Release the color shader object.
	if(m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	if (m_Sphere)
	{
		m_Sphere->Shutdown();
		delete m_Sphere;
		m_Sphere = 0;
	}

	if (m_Cube)
	{
		m_Cube->Shutdown();
		delete m_Cube;
		m_Cube = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	//Release the Direct3D object
	if(m_D3D){

		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	if (m_Quad)
	{
		m_Quad->Shutdown();
		delete m_Quad;
		m_Quad = 0;
	}

	if (m_Cone)
	{
		m_Cone->Shutdown();
		delete m_Cone;
		m_Cone = 0;
	}

	if (m_Cylinder)
	{
		m_Cylinder->Shutdown();
		delete m_Cylinder;
		m_Cylinder = 0;
	}

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	//Release the bump map shader
	if (m_BumpMapShader){
		m_BumpMapShader->Shutdown();
		delete m_BumpMapShader;
		m_BumpMapShader = 0;
	}

	if (m_Torus)
	{
		m_Torus->Shutdown();
		delete m_Torus;
		m_Torus = 0;
	}

	return;
}

bool GraphicsClass::Frame(){

	bool result;

	//Render the graphics scene
	result = Render();
	if(!result)
		return false;
	return true;
}

bool GraphicsClass::Render(){
	
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, RotationZ;
	bool result;
	rotation += .001f;
	rotaCono += 0.001f;

	m_D3D->BeginScene(0.0f,0.0f,0.0f,1.0f);

	m_Camera->Render();

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);


	//////////////ESFERA
	/*m_Sphere->SetScale(D3DXVECTOR3(3, 3, 3));
	m_Sphere->SetPosition(D3DXVECTOR3(5, -1, -10));
	m_Sphere->Render(m_D3D->GetDeviceContext());
	
	result = m_TextureShader2->Render(m_D3D->GetDeviceContext(), m_Sphere->GetIndexCount(), m_Sphere->GetMatrix(), viewMatrix, projectionMatrix, m_Texture4->GetTexture());
	if (!result)
		return false;*/
	////////////FIN ESFERA

	//////////////ESFERA
	/*m_Sphere->SetScale(D3DXVECTOR3(3, 3, 3));
	m_Sphere->SetPosition(D3DXVECTOR3(15, -1, -10));
	m_Sphere->Render(m_D3D->GetDeviceContext());

	result = m_TextureShader3->Render(m_D3D->GetDeviceContext(), m_Sphere->GetIndexCount(), m_Sphere->GetMatrix(), viewMatrix, projectionMatrix, m_Texture4->GetTexture());
	if (!result)
		return false;*/
	////////////FIN ESFERA

	////////////////FLOOR
	m_Quad->SetScale(D3DXVECTOR3(15, 15, 0));
	m_Quad->SetPosition(D3DXVECTOR3(10.0f, -10.0f, 5));
	m_Quad->SetRotation(D3DXVECTOR3(2.0f, 0, 0));
	m_Quad->Render(m_D3D->GetDeviceContext());
	m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Quad->GetIndexCount(), m_Quad->GetMatrix(), viewMatrix, projectionMatrix, m_Texture3->GetTexture());
	////////////////FIN FLOOR

	///////////////ANUNCIOS
	/*int distance = 10;
	for (int i = 0; i < 3; i++){
		m_Cube->SetScale(D3DXVECTOR3(0.5f, 4.0f, 0.5f));
		m_Cube->SetPosition(D3DXVECTOR3((i * distance), 0, 0));
		m_Cube->SetRotation(D3DXVECTOR3(0, 0, 0));
		m_Cube->Render(m_D3D->GetDeviceContext());

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Cube->GetIndexCount(), m_Cube->GetMatrix(), viewMatrix, projectionMatrix, m_Texture1->GetTexture());
		if (!result)
			return false;

		m_Quad->SetScale(D3DXVECTOR3(3, 2, 1));
		m_Quad->SetPosition(D3DXVECTOR3((i * distance), 6, 0));
		m_Quad->SetRotation(D3DXVECTOR3(0, 0, 0));
		m_Quad->Render(m_D3D->GetDeviceContext());
		m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Quad->GetIndexCount(), m_Quad->GetMatrix(), viewMatrix, projectionMatrix, m_Texture2->GetTexture());
	}*/
	///////////////FIN ANUNCIOS

	/////////////cono
	
	m_Cone->SetRotation(D3DXVECTOR3(0.0f, rotaCono, 0.0f));
	m_Cone->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -15.0f));
	m_Cone->Render(m_D3D->GetDeviceContext());

	m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Cone->GetIndexCount(), m_Cone->GetMatrix(), viewMatrix, projectionMatrix,
		m_Cone->GetTexture());

	//m_BumpMapShader->Render(m_D3D->GetDeviceContext(), m_Cone->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		//m_Model->GetTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor());

	/*m_LightShader->Render(m_D3D->GetDeviceContext(), m_Cone->GetIndexCount(), m_Cone->GetMatrix(), viewMatrix, projectionMatrix,
		m_Cone->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());*/
	/////////////FIN cono

	///////////////bump figura
	m_Model->Render(m_D3D->GetDeviceContext());
	m_Cylinder->SetScale(D3DXVECTOR3(5, 5, 5));
	m_Cylinder->SetRotation(D3DXVECTOR3(0, rotaCono, 0));
	m_Cylinder->SetPosition(D3DXVECTOR3(0, 3, 3));
	result = m_BumpMapShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), m_Cylinder->GetMatrix(), viewMatrix, projectionMatrix,
		m_Model->GetTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
	if (!result)
		return false;
	////////////////fin bump

	//////////torus
	m_Torus->SetRotation(D3DXVECTOR3(0, rotaCono, 0));
	
	m_Torus->Render(m_D3D->GetDeviceContext());

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Torus->GetIndexCount(), m_Torus->GetMatrix(), viewMatrix, projectionMatrix,
		m_Torus->GetTexture());
	/////////fin torus

	//////////base
	m_Cylinder->SetPosition(D3DXVECTOR3(0, -2, 0));
	m_Cylinder->SetScale(D3DXVECTOR3(10, 0.5f, 10));
	m_Cylinder->SetRotation(D3DXVECTOR3(0, 0, 0));

	m_Cylinder->Render(m_D3D->GetDeviceContext());

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Cylinder->GetIndexCount(), m_Cylinder->GetMatrix(), viewMatrix, projectionMatrix,
		m_Cylinder->GetTexture());
	/////////fin base

	//////////torus
	int posx[4] = {15.0f, -15.0f, 0, 0};
	int posz[4] = { 0, 0, 15, -15 };
	for (int i = 0; i < 5; i++){
		m_Cylinder->SetPosition(D3DXVECTOR3(posx[i], 0, posz[i]));
		m_Cylinder->SetRotation(D3DXVECTOR3(0, rotaCono*0.5f, 0));
		m_Cylinder->SetScale(D3DXVECTOR3(0.3f, 4, 0.3f));

		m_Cylinder->Render(m_D3D->GetDeviceContext());

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Cylinder->GetIndexCount(), m_Cylinder->GetMatrix(), viewMatrix, projectionMatrix,
			m_Cylinder->GetTexture());
	}
	/////////fin torus

	//////////caballos
	for (int i = 0; i < 5; i++){
		m_Cube->SetPosition(D3DXVECTOR3(posx[i], 2, posz[i]));
		m_Cube->SetRotation(D3DXVECTOR3(0, rotaCono*0.5f, 0));
		m_Cube->SetScale(D3DXVECTOR3(3, 1, 3));

		m_Cube->Render(m_D3D->GetDeviceContext());

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Cube->GetIndexCount(), m_Cube->GetMatrix(), viewMatrix, projectionMatrix,
			m_Cube->GetTexture());
	}
	/////////fin caballos

	//Present the rendered scene to the sceen
	m_D3D->EndScene();
	return true;
}
