/////////////////////////////////////
//Filename: graphicsclass.h
/////////////////////////////////////

#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

////////////
//INCLUDES//
////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "sphereclass.h"
#include "cubeclass.h"
#include "colorshaderclass.h"
#include "textureshaderclass.h"
#include "bumpmapshaderclass.h"
#include "textureclass.h"
#include "QuadClass.h"
#include "cylinderclass.h"
#include "torusclass.h"
#include "coneclass.h"

#include "lightclass.h"
#include "lightshaderclass.h"

#include "modelclass.h"
#include "bumpmapshaderclass.h"

///////////
//GLOBALS//
///////////

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////////////////////////
//Class name: GraphicsClass
////////////////////////////////////////
class GraphicsClass{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:

	D3DClass* m_D3D;
	cameraclass* m_Camera;
	SphereClass* m_Sphere;
	CubeClass* m_Cube;
	QuadClass* m_Quad;
	ConeClass* m_Cone;
	CylinderClass* m_Cylinder;
	TorusClass* m_Torus;
	ColorShaderClass* m_ColorShader;
	TextureShaderClass* m_TextureShader, *m_TextureShader2, *m_TextureShader3;
	TextureClass* m_Texture1, *m_Texture2, *m_Texture3, *m_Texture4;

	ModelClass* m_Model;

	BumpMapShaderClass* m_BumpMapShader;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	float rotaCono;
};

#endif
