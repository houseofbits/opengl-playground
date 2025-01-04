#include "./FBXInfo.h"
#include "DisplayCommon.h"
#include "DisplayHierarchy.h"
#include "DisplayAnimation.h"
#include "DisplayMarker.h"
#include "DisplaySkeleton.h"
#include "DisplayMesh.h"
#include "DisplayNurb.h"
#include "DisplayPatch.h"
#include "DisplayLodGroup.h"
#include "DisplayCamera.h"
#include "DisplayLight.h"
#include "DisplayGlobalSettings.h"
#include "DisplayPose.h"
#include "DisplayPivotsAndLimits.h"
#include "DisplayUserProperties.h"
#include "DisplayGenericInfo.h"

void FBXInfo::DisplayContent(const FbxScene *pScene) {
    if (FbxNode *lNode = pScene->GetRootNode()) {
        for (int i = 0; i < lNode->GetChildCount(); i++) {
            FBXInfo::DisplayContent(lNode->GetChild(i));
        }
    }
}

void FBXInfo::DisplayContent(FbxNode *pNode) {
    if (pNode->GetNodeAttribute() == nullptr) {
        FBXSDK_printf("NULL Node Attribute\n\n");
    } else {
        switch (pNode->GetNodeAttribute()->GetAttributeType()) {
            default:
                break;
            case FbxNodeAttribute::eMarker:
                DisplayMarker(pNode);
                break;

            case FbxNodeAttribute::eSkeleton:
                DisplaySkeleton(pNode);
                break;

            case FbxNodeAttribute::eMesh:
                DisplayMesh(pNode);
                break;

            case FbxNodeAttribute::eNurbs:
                DisplayNurb(pNode);
                break;

            case FbxNodeAttribute::ePatch:
                DisplayPatch(pNode);
                break;

            case FbxNodeAttribute::eCamera:
                DisplayCamera(pNode);
                break;

            case FbxNodeAttribute::eLight:
                DisplayLight(pNode);
                break;

            case FbxNodeAttribute::eLODGroup:
                DisplayLodGroup(pNode);
                break;
        }
    }

    DisplayUserProperties(pNode);
    DisplayTarget(pNode);
    DisplayPivotsAndLimits(pNode);
    DisplayTransformPropagation(pNode);
    DisplayGeometricTransform(pNode);

    for (int i = 0; i < pNode->GetChildCount(); i++) {
        FBXInfo::DisplayContent(pNode->GetChild(i));
    }
}

void FBXInfo::DisplayTarget(const FbxNode *pNode) {
    if (pNode->GetTarget() != nullptr) {
        DisplayString("    Target Name: ", (char *) pNode->GetTarget()->GetName());
    }
}

void FBXInfo::DisplayTransformPropagation(const FbxNode *pNode) {
    FBXSDK_printf("    Transformation Propagation\n");

    //
    // Rotation Space
    //
    EFbxRotationOrder lRotationOrder;
    pNode->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);

    FBXSDK_printf("        Rotation Space: ");

    switch (lRotationOrder) {
        case eEulerXYZ:
            FBXSDK_printf("Euler XYZ\n");
            break;
        case eEulerXZY:
            FBXSDK_printf("Euler XZY\n");
            break;
        case eEulerYZX:
            FBXSDK_printf("Euler YZX\n");
            break;
        case eEulerYXZ:
            FBXSDK_printf("Euler YXZ\n");
            break;
        case eEulerZXY:
            FBXSDK_printf("Euler ZXY\n");
            break;
        case eEulerZYX:
            FBXSDK_printf("Euler ZYX\n");
            break;
        case eSphericXYZ:
            FBXSDK_printf("Spheric XYZ\n");
            break;
    }

    //
    // Use the Rotation space only for the limits
    // (keep using eEulerXYZ for the rest)
    //
    FBXSDK_printf("        Use the Rotation Space for Limit specification only: %s\n",
                  pNode->GetUseRotationSpaceForLimitOnly(FbxNode::eSourcePivot) ? "Yes" : "No");


    //
    // Inherit Type
    //
    FbxTransform::EInheritType lInheritType;
    pNode->GetTransformationInheritType(lInheritType);

    FBXSDK_printf("        Transformation Inheritance: ");

    switch (lInheritType) {
        case FbxTransform::eInheritRrSs:
            FBXSDK_printf("RrSs\n");
            break;
        case FbxTransform::eInheritRSrs:
            FBXSDK_printf("RSrs\n");
            break;
        case FbxTransform::eInheritRrs:
            FBXSDK_printf("Rrs\n");
            break;
    }
}

void FBXInfo::DisplayGeometricTransform(const FbxNode *pNode) {
    FBXSDK_printf("    Geometric Transformations\n");

    //
    // Translation
    //
    FbxVector4 lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    FBXSDK_printf("        Translation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Rotation
    //
    lTmpVector = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
    FBXSDK_printf("        Rotation:    %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Scaling
    //
    lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
    FBXSDK_printf("        Scaling:     %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);
}

void FBXInfo::DisplayMetaData(FbxScene *pScene) {
    if (FbxDocumentInfo *sceneInfo = pScene->GetSceneInfo()) {
        FBXSDK_printf("\n\n--------------------\nMeta-Data\n--------------------\n\n");
        FBXSDK_printf("    Title: %s\n", sceneInfo->mTitle.Buffer());
        FBXSDK_printf("    Subject: %s\n", sceneInfo->mSubject.Buffer());
        FBXSDK_printf("    Author: %s\n", sceneInfo->mAuthor.Buffer());
        FBXSDK_printf("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
        FBXSDK_printf("    Revision: %s\n", sceneInfo->mRevision.Buffer());
        FBXSDK_printf("    Comment: %s\n", sceneInfo->mComment.Buffer());

        if (FbxThumbnail *thumbnail = sceneInfo->GetSceneThumbnail()) {
            FBXSDK_printf("    Thumbnail:\n");

            switch (thumbnail->GetDataFormat()) {
                case FbxThumbnail::eRGB_24:
                    FBXSDK_printf("        Format: RGB\n");
                    break;
                case FbxThumbnail::eRGBA_32:
                    FBXSDK_printf("        Format: RGBA\n");
                    break;
            }

            switch (thumbnail->GetSize()) {
                default:
                    break;
                case FbxThumbnail::eNotSet:
                    FBXSDK_printf("        Size: no dimensions specified (%ld bytes)\n", thumbnail->GetSizeInBytes());
                    break;
                case FbxThumbnail::e64x64:
                    FBXSDK_printf("        Size: 64 x 64 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
                    break;
                case FbxThumbnail::e128x128:
                    FBXSDK_printf("        Size: 128 x 128 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
            }
        }
    }
}

void FBXInfo::Display(FbxScene *lScene) {
    DisplayMetaData(lScene);

    FBXSDK_printf("\n\n---------------------\nGlobal Light Settings\n---------------------\n\n");

    DisplayGlobalLightSettings(&lScene->GetGlobalSettings());

    FBXSDK_printf("\n\n----------------------\nGlobal Camera Settings\n----------------------\n\n");

    DisplayGlobalCameraSettings(&lScene->GetGlobalSettings());

    FBXSDK_printf("\n\n--------------------\nGlobal Time Settings\n--------------------\n\n");

    DisplayGlobalTimeSettings(&lScene->GetGlobalSettings());

    FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");

    DisplayHierarchy(lScene);

    FBXSDK_printf("\n\n------------\nNode Content\n------------\n\n");

    DisplayContent(lScene);

    FBXSDK_printf("\n\n----\nPose\n----\n\n");

    DisplayPose(lScene);

    FBXSDK_printf("\n\n---------\nAnimation\n---------\n\n");

    DisplayAnimation(lScene);

    FBXSDK_printf("\n\n---------\nGeneric Information\n---------\n\n");
    DisplayGenericInfo(lScene);
}
