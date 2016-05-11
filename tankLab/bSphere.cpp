#include "bSphere.h"

BoundingSphere::BoundingSphere() {}

void BoundingSphere::createBoundingSphere(ObjMesh * objL[], int lAmount) {
	float x, y, z;
	float sphereRadius = 0.0;
	ObjVertex vp_avg;
	vp_avg.x = 0.0;
	vp_avg.y = 0.0;
	vp_avg.z = 0.0;
	for (int m = 0; m < lAmount; ++m) {
		ObjMesh * obj = objL[m];
	
		for (int i = 0; i < obj->m_iNumberOfVertices; ++i) {
			vp_avg.x += obj->m_aVertexArray[i].x;
			vp_avg.y += obj->m_aVertexArray[i].y;
			vp_avg.z += obj->m_aVertexArray[i].z;
		}
		vp_avg.x /= obj->m_iNumberOfVertices;
		vp_avg.y /= obj->m_iNumberOfVertices;
		vp_avg.z /= obj->m_iNumberOfVertices;

		for (int i = 0; i < obj->m_iNumberOfVertices; ++i) {
			x = obj->m_aVertexArray[i].x;
			y = obj->m_aVertexArray[i].y;
			z = obj->m_aVertexArray[i].z;
			float d = sqrt(pow(vp_avg.x - x, 2) + pow(vp_avg.y - y, 2) + pow(vp_avg.z - z, 2));
			if (d > sphereRadius) sphereRadius = d;
		}
	}
	this->centerX = vp_avg.x;
	this->centerY = vp_avg.y;
	this->centerZ = vp_avg.z;
	this->radius = sphereRadius;
}

