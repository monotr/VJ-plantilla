#include "cameraclass.h"


cameraclass::cameraclass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

cameraclass::cameraclass(const cameraclass& other)
{

}

cameraclass::~cameraclass()
{
}

void cameraclass::SetPosition(float x, float y, float z){
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void cameraclass::SetRotation(float x, float y, float z){
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

D3DXVECTOR3 cameraclass::GetPosition(){
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 cameraclass::GetRotation(){
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void cameraclass::Render(){
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll; //camera rotation
	D3DXMATRIX rotationMatrix;

	//setup the vector that points upwards
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//setup the position of the camera in the world
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	//setup where the camera is looking by default
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	//set the yaw(Y axis), pitch(X axis) and roll(Z axis) rotations in radians
	pitch = m_rotationX * 0.0174532925f;
	yaw	  = m_rotationY * 0.0174532925f;
	roll  = m_rotationZ * 0.0174532925f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	//transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	//translate the rotated camera position to the location of the view
	lookAt = position + lookAt;

	//finally create the view matrix from the 3 updated vectors
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}

void cameraclass::GetViewMatrix(D3DXMATRIX &viewMatrix){
	viewMatrix = m_viewMatrix;
	return;
}


