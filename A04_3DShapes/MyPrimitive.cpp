#include "MyPrimitive.h"
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
/// Added by Ian Hampson
/*
	Creates A TRI of b_left->b_Right->t_Center
	follows the right hand rule of OpenGL
*/
void MyPrimitive::AddTri(vector3 b_Left, vector3 b_Right, vector3 t_Center)
{
	AddVertexPosition(b_Left);
	AddVertexPosition(b_Right);
	AddVertexPosition(t_Center);
}
void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	/// My code starts here
	float fRad = 0.5f*a_fRadius;
	float fHeight = 0.5f*a_fHeight;
	float degreeDif = 360 / a_nSubdivisions;
	bool center = (a_nSubdivisions <= 4) ? false : true;

	std::vector<vector3> verts;
	vector3 cTip(0, fHeight, 0);
	vector3 cBase(0.0f, -fHeight, 0.0f);
	/// Adding vectors to verts
	for (int i = 0; i <= a_nSubdivisions; i++) {
		vector3 temp(fRad * cos(i*degreeDif * PI/180), -fHeight, fRad * sin(i*degreeDif * PI/180));
		verts.push_back(temp);
	}

	for (int j = 0; j <= a_nSubdivisions; j++){
		if (j == a_nSubdivisions){
			AddTri(verts[j], verts[0], cTip);
		}	else{
			AddTri(verts[j], verts[j + 1], cTip);
		}
	}

	if (center) {
		for (int k = 0; k <= a_nSubdivisions; k++) {
			if (k == a_nSubdivisions) {
				AddTri(cBase,verts[0], verts[k]);
			}
			else {
				AddTri(cBase, verts[k + 1], verts[k]);
			}
		}
	}
	else
	{
		switch (a_nSubdivisions) {
		case 3: 
			AddTri(verts[2], verts[1], verts[0]);
			break;
		case 4:
			AddTri(verts[2], verts[1], verts[0]);
			AddTri(verts[2], verts[0], verts[3]);
			break;
		}
	}
	/// My code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	/// My code starts here
	float fRad = 0.5f*a_fRadius;
	float fHeight = 0.5f*a_fHeight;
	float degreeDif = 360 / a_nSubdivisions;
	bool center = (a_nSubdivisions <= 4) ? false : true;

	std::vector<vector3> topVerts;
	std::vector<vector3> botVerts; 
	vector3 cTop(0, fHeight, 0);
	vector3 cBase(0.0f, -fHeight, 0.0f);
	/// Adding vectors to verts
	for (int i = 0; i < a_nSubdivisions; i++) {
		vector3 temp(fRad * cos(i*degreeDif * PI / 180), fHeight, fRad * sin(i*degreeDif * PI / 180));
		topVerts.push_back(temp);
	}
	
	for (int j = 0; j < a_nSubdivisions; j++) {
		vector3 temp(fRad * cos(j*degreeDif * PI / 180), -fHeight, fRad * sin(j*degreeDif * PI / 180));
		botVerts.push_back(temp);
	}

	if (center) {
		/// Draw the top
		for (int j = 0; j < a_nSubdivisions; j++) {
			if (j == a_nSubdivisions-1) {
				AddTri(topVerts[j], topVerts[0], cTop);
			}
			else {
				AddTri(topVerts[j], topVerts[j + 1], cTop);
			}
		}
	
		/// Draw the bottom
		for (int k = 0; k < a_nSubdivisions; k++) {
			if (k == a_nSubdivisions-1) {
				AddTri(cBase, botVerts[0], botVerts[k]);
			}
			else {
				AddTri(cBase, botVerts[k + 1], botVerts[k]);
			}
		}
		/// Draw the walls
		for (int i = 0; i < a_nSubdivisions; i++) {
			if (i == a_nSubdivisions-1) {
				AddQuad(botVerts[i], botVerts[0], topVerts[i], topVerts[0]);
			}
			else {
				AddQuad(botVerts[i], botVerts[i+1], topVerts[i], topVerts[i+1]);
			}
		}
	}
	else {
		switch (a_nSubdivisions) {
		case 3:
			AddTri(topVerts[0], topVerts[1], topVerts[2]);
			AddTri(botVerts[2], botVerts[1], botVerts[0]);
			AddQuad(botVerts[0], botVerts[1], topVerts[0], topVerts[1]);
			AddQuad(botVerts[1], botVerts[2], topVerts[1], topVerts[2]);
			AddQuad(botVerts[2], botVerts[0], topVerts[2], topVerts[0]);
			break;
		case 4:
			AddQuad(topVerts[0], topVerts[1], topVerts[3], topVerts[2]);
			AddQuad(botVerts[2], botVerts[3], botVerts[1], botVerts[0]); 
			break;
		}
	}
	
	/// My code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	/// My code starts here
	float fOutRad = 0.5f*a_fOuterRadius;
	float fInRad = 0.5f*a_fInnerRadius;
	float fHeight = 0.5f*a_fHeight;
	float degreeDif = 360 / a_nSubdivisions;

	std::vector<vector3> topOuterVerts;
	std::vector<vector3> topInnerVerts;
	std::vector<vector3> botOuterVerts;
	std::vector<vector3> botInnerVerts;
	//vector3 cTop(0, fHeight, 0);
	//vector3 cBase(0.0f, -fHeight, 0.0f);

	/// Adding vectors to verts
	for (int i = 0; i <= a_nSubdivisions; i++) {
		vector3 tempOut(fOutRad * cos(i*degreeDif * PI / 180), fHeight, fOutRad * sin(i*degreeDif * PI / 180));
		topOuterVerts.push_back(tempOut);
		vector3 tempIn(fInRad * cos(i*degreeDif * PI / 180), fHeight, fInRad * sin(i*degreeDif * PI / 180));
		topInnerVerts.push_back(tempIn);
	}

	for (int j = 0; j <= a_nSubdivisions; j++) {
		vector3 tempOut(fOutRad * cos(j*degreeDif * PI / 180), -fHeight, fOutRad * sin(j*degreeDif * PI / 180));
		botOuterVerts.push_back(tempOut);
		vector3 tempIn(fInRad * cos(j*degreeDif * PI / 180), -fHeight, fInRad * sin(j*degreeDif * PI / 180));
		botInnerVerts.push_back(tempIn);
	}

	/// Draw the top and bottom rings
	/// Plus draws the outer and inner walls for each increment.
	for (int i = 0; i <= a_nSubdivisions; i++) {
		if (i == a_nSubdivisions) {
			AddQuad(topOuterVerts[i], topOuterVerts[0], topInnerVerts[i], topInnerVerts[0]);
			AddQuad(botOuterVerts[0], botOuterVerts[i], botInnerVerts[0], botInnerVerts[i]);
			AddQuad(botOuterVerts[i], botOuterVerts[0], topOuterVerts[i], topOuterVerts[0]);
			AddQuad(botInnerVerts[0], botInnerVerts[i], topInnerVerts[0], topInnerVerts[i]);
		}
		else {
			AddQuad(topOuterVerts[i], topOuterVerts[i + 1], topInnerVerts[i], topInnerVerts[i + 1]);
			AddQuad(botOuterVerts[i + 1], botOuterVerts[i], botInnerVerts[i + 1], botInnerVerts[i]);
			AddQuad(botOuterVerts[i], botOuterVerts[i + 1], topOuterVerts[i], topOuterVerts[i + 1]);
			AddQuad(botInnerVerts[i + 1], botInnerVerts[i], topInnerVerts[i + 1], topInnerVerts[i]);
		}
	}
	
	/// My code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	AddQuad(point0, point1, point3, point2);

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions <= 2)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 10)
		a_nSubdivisions = 8;

	Release();
	Init();

	/// My code starts here
	float fRad = a_fRadius;
	float increment = (2 * fRad) / (a_nSubdivisions-1);
	float height = fRad+(increment/2);
	float ringInc = (2 * PI) / a_nSubdivisions;

	vector3 cTop(0.0f, fRad, 0.0f);
	vector3 cBot(0.0f, -fRad, 0.0f);
	std::vector<vector3> prevRing;

	for (int i = 1; i < a_nSubdivisions; i++) {
		height -= increment;
		float ringRad = cos(asin(height/fRad))*fRad;
		std::vector<vector3> currentRing;
	
		/// Generate verts in this ring
		float x, y, z;
		for (int j = 0; j < a_nSubdivisions; j++) {
			x = ringRad*cos(j*ringInc);
			y = height;
			z = ringRad*sin(j*ringInc);
			currentRing.push_back(vector3(x,y,z));
		}

		/// Attatch verts together
		if (i == 1) {
			for (int k = 0; k < a_nSubdivisions; k++) {
				if (k == a_nSubdivisions - 1) {
					AddTri(currentRing[k], currentRing[0], cTop);
				}
				else {
					AddTri(currentRing[k], currentRing[k + 1], cTop);
				}
			}
		}
		else {
			for (int k = 0; k < a_nSubdivisions; k++) {
				if (k == a_nSubdivisions-1) {
					AddQuad(currentRing[k], currentRing[0], prevRing[k], prevRing[0]);
				}
				else {
					AddQuad(currentRing[k], currentRing[k + 1], prevRing[k], prevRing[k + 1]);
				}
			}
		}
		prevRing = currentRing;
	}
	for (int i = 0; i < a_nSubdivisions; i++) {
		if (i == a_nSubdivisions - 1) {
			AddTri(cBot, prevRing[0], prevRing[i]);
		}
		else {
			AddTri(cBot, prevRing[i + 1], prevRing[i]);
		}
	}

	/// My code ends here
	CompileObject(a_v3Color);
}