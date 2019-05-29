#include "Transform.h"


#define GLM_FORCE_
#define M_SQRT2 1.41421356237309504880 
#include <engine/physx/PhysicSystem.h>
#include<math.h>

Transform::Transform() : localPosition(0,0,0), localScale(1,1,1), localeulerRotation(0,0,0), worldMatrix(1), localMatrix(1), physicactor(nullptr)
{
	calcQuatFromEuler();
}

void Transform::decomposeLocalMatrix()
{
	glm::vec3 skew(1);
	glm::vec4 perspective(1);
	glm::decompose(localMatrix, localScale, localquatRotation, localPosition, skew, perspective);
	

		
	if(GE_Engine->physicSystem->enabled)
		calcEulerFromQuat();
}

glm::vec3 ConvertMatToEulerAnglesXYZ(const glm::mat3& mat)
{
	const float xEuler = std::atan2(mat[1].z, mat[2].z);
	const float cosYangle = glm::sqrt(glm::pow(mat[0].x, 2) + glm::pow(mat[0].y, 2));
	const float yEuler = std::atan2(-mat[0].z, cosYangle);
	const float sinXangle = glm::sin(xEuler);
	const float cosXangle = glm::cos(xEuler);
	const float zEuler = std::atan2(
		sinXangle * mat[2].x - cosXangle * mat[1].x,
		cosXangle * mat[1].y - sinXangle * mat[2].y);

	return glm::vec3(xEuler, yEuler, zEuler);
}
static void quat_to_mat3_no_error(glm::mat3 &m,glm::quat &q)
{
	double q0, q1, q2, q3, qda, qdb, qdc, qaa, qab, qac, qbb, qbc, qcc;
	
	q0 = M_SQRT2 * (double)q.w;
		q1 = M_SQRT2 * (double)q.x;
	q2 = M_SQRT2 * (double)q.y;
	q3 = M_SQRT2 * (double)q.z;

	qda = q0 * q1;
	qdb = q0 * q2;
	qdc = q0 * q3;
	qaa = q1 * q1;
	qab = q1 * q2;
	qac = q1 * q3;
	qbb = q2 * q2;
	qbc = q2 * q3;
	qcc = q3 * q3;

	m[0][0] = (float)(1.0 - qbb - qcc);
	m[0][1] = (float)(qdc + qab);
	m[0][2] = (float)(-qdb + qac);

	m[1][0] = (float)(-qdc + qab);
	m[1][1] = (float)(1.0 - qaa - qcc);
	m[1][2] = (float)(qda + qbc);

	m[2][0] = (float)(qdb + qac);
	m[2][1] = (float)(-qda + qbc);
	m[2][2] = (float)(1.0 - qaa - qbb);
}
void getEulerZYX(glm::mat3 mat, float eul1[3], float eul2[3])
{
	const float cy = hypotf(mat[0][0], mat[0][1]);
	if (cy > 16.0f * FLT_EPSILON) {

		eul1[0] = atan2f(mat[1][2], mat[2][2]);
		eul1[1] = atan2f(-mat[0][2], cy);
		eul1[2] = atan2f(mat[0][1], mat[0][0]);

		eul2[0] = atan2f(-mat[1][2], -mat[2][2]);
		eul2[1] = atan2f(-mat[0][2], -cy);
		eul2[2] = atan2f(-mat[0][1], -mat[0][0]);
	}
	else {
		eul1[0] = atan2f(-mat[2][1], mat[1][1]);
		eul1[1] = atan2f(-mat[0][2], cy);
		eul1[2] = 0.0f;


		eul2[0] = atan2f(-mat[2][1], mat[1][1]);
		eul2[1] = atan2f(-mat[0][2], cy);
		eul2[2] = 0.0f;
		
	}


}
void Transform::calcEulerFromQuat()
{
	glm::mat3 m3;

	quat_to_mat3_no_error(m3, localquatRotation);

	float x[3], y[3];
	getEulerZYX(m3, x, y);

	if (fabsf(x[0]) + fabsf(x[1]) + fabsf(x[2]) >
		fabsf(y[0]) + fabsf(y[1]) + fabsf(y[2])) {
		localeulerRotation.x = y[0];
		localeulerRotation.y = y[1];
		localeulerRotation.z = y[2];
	}
	else {
		localeulerRotation.x = x[0];
		localeulerRotation.y = x[1];
		localeulerRotation.z = x[2];
	}

	
	localeulerRotation = glm::degrees(localeulerRotation);
	//glm::vec3 test = glm::degrees(glm::eulerAngles(glm::normalize(localquatRotation)));
	//localeulerRotation = test;
	
}
void eul_to_quat(glm::quat &quat, const float eul[3])
{
	float ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;

	ti = eul[0] * 0.5f;
	tj = eul[1] * 0.5f;
	th = eul[2] * 0.5f;
	ci = cosf(ti);
	cj = cosf(tj);
	ch = cosf(th);
	si = sinf(ti);
	sj = sinf(tj);
	sh = sinf(th);
	cc = ci * ch;
	cs = ci * sh;
	sc = si * ch;
	ss = si * sh;

	quat.w = cj * cc + sj * ss;
	quat.x = cj * sc - sj * cs;
	quat.y = cj * ss + sj * cc;
	quat.x = cj * cs - sj * sc;
}
void Transform::calcQuatFromEuler()
{

	localquatRotation = glm::quat(glm::radians(localeulerRotation));
}




glm::vec3 Transform::getWorldPosition()
{
	glm::vec3 pos;
	glm::vec4 test;
	glm::quat testq;
	glm::vec3 testp;
	glm::decompose(worldMatrix, testp, testq, pos, testp, test);
	return pos;
}

glm::vec3 Transform::getWorldScale()
{
	glm::vec3 pos;
	glm::vec4 test;
	glm::quat testq;
	glm::vec3 scale;
	glm::decompose(worldMatrix, scale, testq, pos, pos, test);
	return scale;
}

glm::quat Transform::getWorldRotation()
{
	glm::vec3 pos;
	glm::vec4 test;
	glm::quat testq;
	glm::vec3 scale;
	glm::decompose(worldMatrix, scale, testq, pos, pos, test);
	return testq;
}

glm::vec3 Transform::getWorldForwardVector()
{
	glm::quat rotation = getWorldRotation();
	glm::vec3 forward;
	forward.x = 2 * (rotation.x *rotation.z + rotation.w* rotation.y);
	forward.y = 2 * (rotation.y *rotation.z - rotation.w* rotation.x);
	forward.z = 1 - 2* (rotation.x *rotation.x + rotation.y* rotation.y);
	return glm::normalize(forward);
}
glm::vec3 Transform::getWorldUpVector()
{
	glm::quat rotation = getWorldRotation();
	glm::vec3 forward;
	forward.x = 2 * (rotation.x *rotation.y - rotation.w* rotation.z);
	forward.y = 1- 2 * (rotation.x *rotation.x + rotation.z* rotation.z);
	forward.z =  2 * (rotation.y *rotation.z + rotation.w* rotation.x);

	return glm::normalize(forward);
}
glm::vec3 Transform::getWorldRightVector()
{
	glm::quat rotation = getWorldRotation();
	glm::vec3 forward;
	forward.x =1- 2 * (rotation.y *rotation.y + rotation.z* rotation.z);
	forward.y =  2 * (rotation.x *rotation.y + rotation.w* rotation.z);
	forward.z = 2 * (rotation.x *rotation.z - rotation.w* rotation.y);
	return glm::normalize(forward);
}

glm::mat4 Transform::getWorldPose()
{
	return glm::translate(glm::mat4(1), localPosition) * (glm::toMat4(localquatRotation));
}

void Transform::applyToRigidBody()
{
	calcLocalMatrix();
	setWorldMatrix(parent->getWorldMatrix() * getLocalMatrix());


	if(this->physicactor)
		this->physicactor->setGlobalPose(PxTransform(glmMat4ToPhysxMat4(getWorldPose())));
}

void Transform::calcLocalMatrix()
{
	glm::mat4 rot = glm::toMat4(localquatRotation);
	glm::mat4 pos = glm::translate(glm::mat4(1.f), localPosition);
	glm::mat4 scale = glm::scale(glm::mat4(1.f), localScale);
	localMatrix = pos * rot*scale;
}



Transform::~Transform()
{
}