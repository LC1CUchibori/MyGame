#include "math.h"
#include <math/Vector4.h>
#include <KamataEngine.h>
using namespace KamataEngine;

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	KamataEngine::Matrix4x4 result = {};
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1.0f;
	return result;
}

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f;
	result.m[1][1] = 1.0f;
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	return result;
}

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float angle) {
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f;
	result.m[1][1] = cos(angle);
	result.m[1][2] = -sin(angle);
	result.m[2][1] = sin(angle);
	result.m[2][2] = cos(angle);
	result.m[3][3] = 1.0f;
	return result;
}

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float angle) {
	Matrix4x4 result = {};
	result.m[0][0] = cos(angle);
	result.m[0][2] = sin(angle);
	result.m[1][1] = 1.0f;
	result.m[2][0] = -sin(angle);
	result.m[2][2] = cos(angle);
	result.m[3][3] = 1.0f;
	return result;
}

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float angle) {
	Matrix4x4 result = {};
	result.m[0][0] = cos(angle);
	result.m[0][1] = -sin(angle);
	result.m[1][0] = sin(angle);
	result.m[1][1] = cos(angle);
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 Multiply(const Matrix4x4& a, const Matrix4x4& b) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return result;
}

Vector4 Multiply(const Matrix4x4& m, const Vector4& v) {
	Vector4 result;
	result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
	result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
	result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
	result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
	return result;
}

KamataEngine::Matrix4x4 Inverse(const KamataEngine::Matrix4x4& m)
{
	KamataEngine::Matrix4x4 inv;

	// 回転部分の転置
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			inv.m[i][j] = m.m[j][i];
		}
	}

	// 平行移動部分
	inv.m[3][0] = -(inv.m[0][0]*m.m[3][0] + inv.m[1][0]*m.m[3][1] + inv.m[2][0]*m.m[3][2]);
	inv.m[3][1] = -(inv.m[0][1]*m.m[3][0] + inv.m[1][1]*m.m[3][1] + inv.m[2][1]*m.m[3][2]);
	inv.m[3][2] = -(inv.m[0][2]*m.m[3][0] + inv.m[1][2]*m.m[3][1] + inv.m[2][2]*m.m[3][2]);

	// 残り
	inv.m[0][3] = inv.m[1][3] = inv.m[2][3] = 0.0f;
	inv.m[3][3] = 1.0f;

	return inv;
}


// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result;
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	result = Multiply(rotateXYZMatrix, Multiply(scaleMatrix, translateMatrix));
	return result;
}

