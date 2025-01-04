#pragma once

#include <fbxsdk.h>

class FBXInfo {
public:
    FBXInfo() = default;

    static void Display(FbxScene *pScene);

    static void DisplayContent(const FbxScene *pScene);

    static void DisplayContent(FbxNode *pScene);

    static void DisplayTarget(const FbxNode *pNode);

    static void DisplayTransformPropagation(const FbxNode *pNode);

    static void DisplayGeometricTransform(const FbxNode *pNode);

    static void DisplayMetaData(FbxScene *pScene);
};
