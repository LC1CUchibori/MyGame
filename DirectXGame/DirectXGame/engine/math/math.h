#pragma once

#include <KamataEngine.h>
#include <math\Matrix4x4.h>
#include <math\Vector3.h>

// アフィン変換行列の作成
KamataEngine::Matrix4x4 MakeAffineMatrix(const KamataEngine::Vector3& scale_, const KamataEngine::Vector3& rotation_, const KamataEngine::Vector3& translation_);

KamataEngine::Matrix4x4 MakeScaleMatrix(const KamataEngine::Vector3& scale);

// 平行移動行列
KamataEngine::Matrix4x4 MakeTranslateMatrix(const KamataEngine::Vector3& translate);

// X軸回転行列
KamataEngine::Matrix4x4 MakeRotateXMatrix(float angle);

// Y軸回転行列
KamataEngine::Matrix4x4 MakeRotateYMatrix(float angle);

// Z軸回転行列
KamataEngine::Matrix4x4 MakeRotateZMatrix(float angle);
