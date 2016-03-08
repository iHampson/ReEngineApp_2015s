#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Sandbox"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(
		vector3(0.0f, 2.5f, 12.0f),
		vector3(0.0f, 2.5f, 11.0f),
		vector3(0.0f, 1.0f, 0.0f)
		);
	srand(time(NULL));
	m_nObjects = rand() % 23 + 5;
	

	vector3 v3Start = vector3(-static_cast<float>(m_nObjects), 0.0f, 0.0f);
	vector3 v3End = vector3(static_cast<float>(m_nObjects), 0.0f, 0.0f);
	

	m_pSphere = new PrimitiveClass[m_nObjects];
	m_pMatrix = new matrix4[m_nObjects];

	for (int i = 0; i < m_nObjects; i++) {
		float fPercent = MapValue(static_cast<float>(i), 0.0f, static_cast<float>(m_nObjects)-1, 0.0f, 1.0f); 
		m_pSphere[i].GenerateSphere(0.5f, 5, RERED);
		vector3 v3Current = glm::lerp(v3Start, v3End, fPercent);
		m_pMatrix[i] = glm::translate(v3Current);
	}

	

}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Call the arcball method
	ArcBall();
	m_pMeshMngr->SetModelMatrix(glm::translate(m_v3Position) * ToMatrix4(m_qArcBall), 0);
	
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	//printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("Selection: ");
	m_pMeshMngr->PrintLine(m_pMeshMngr->GetInstanceGroupName(m_selection.first, m_selection.second), REYELLOW);
	
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	matrix4 mProjection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 mView = m_pCameraMngr->GetViewMatrix();

	for (int i = 0; i < m_nObjects; i++) {
		m_pSphere[i].Render(mProjection, mView, m_pMatrix[i]);
	}

	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
	if (m_pSphere != nullptr) {
		delete[] m_pSphere;
		m_pSphere = nullptr;
	}
	if (m_pMatrix != nullptr) {
		delete[] m_pMatrix;
		m_pMatrix = nullptr;
	}
}