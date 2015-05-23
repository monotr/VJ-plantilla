///file name: cameraclass.h
//#ifdef _CAMERACLASS_H_
#define _CAMERACLASS_H_

////INCLUDES
#include <D3DX10math.h>

///class name: camerclass

class cameraclass
{
public:
	cameraclass();
	cameraclass(const cameraclass&);
	~cameraclass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
};

//#endif

