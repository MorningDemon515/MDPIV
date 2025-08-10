#include "Animator.h"
#include "AssimpConvert.h"

using namespace MD_Math;

void Animator::UpdateAnimation(float time)
{
    if(scene->mNumAnimations == 0) return;
        
    float ticksPerSecond = scene->mAnimations[0]->mTicksPerSecond != 0 ? 
                            scene->mAnimations[0]->mTicksPerSecond : 25.0f;
    float timeInTicks = time * ticksPerSecond;
    animationTime = fmod(timeInTicks, (float)scene->mAnimations[0]->mDuration);
        
    globalInverseTransform = MatrixTranspose(InvMatrix(ConvertMatrix(scene->mRootNode->mTransformation)));
    ReadNodeHierarchy(animationTime, scene->mRootNode, IdentityMatrix());
}

void Animator::SetShader(Shader& shader)
{
    std::vector<MATRIX> transforms;
    for(unsigned int i = 0; i < boneInfoList.size(); i++) {
        transforms.push_back(boneInfoList[i].FinalTransformation);
    }
        
    for(unsigned int i = 0; i < transforms.size(); i++) {
        MATRIX transposed = transforms[i];
        shader.SetMatrix(("boneTransforms[" + std::to_string(i) + "]").c_str(), transposed);
    }
}

void Animator::ReadNodeHierarchy(float animationTime, const aiNode* node, const MD_Math::MATRIX& parentTransform)
{
    std::string nodeName(node->mName.data);
        MD_Math::MATRIX nodeTransform = ConvertMatrix(node->mTransformation);
        
        const aiAnimation* animation = scene->mAnimations[0];
        const aiNodeAnim* nodeAnim = FindNodeAnim(animation, nodeName);
        
        if(nodeAnim) {
   
            aiVector3D scaling = CalcInterpolatedScaling(animationTime, nodeAnim);
            aiQuaternion rotation = CalcInterpolatedRotation(animationTime, nodeAnim);
            aiVector3D translation = CalcInterpolatedPosition(animationTime, nodeAnim);
            
            MD_Math::MATRIX scalingM = ScaleMatrix(scaling.x, scaling.y, scaling.z);
            MD_Math::QUATERNION quat(rotation.x, rotation.y, rotation.z, rotation.w);
            MD_Math::MATRIX rotationM = MatrixFromQuaternion(quat);
            MD_Math::MATRIX translationM = TranslationMatrix(translation.x, translation.y, translation.z);
            
            nodeTransform = translationM * rotationM * scalingM;
        }
        
        MD_Math::MATRIX PT = parentTransform;
        MD_Math::MATRIX globalTransform = PT * nodeTransform;
        
        if(boneMapping.find(nodeName) != boneMapping.end()) {
            unsigned int boneID = boneMapping[nodeName];
            boneInfoList[boneID].FinalTransformation = 
                globalInverseTransform * 
                globalTransform * 
                boneInfoList[boneID].Offset;
        }
        
        for(unsigned int i = 0; i < node->mNumChildren; i++) {
            ReadNodeHierarchy(animationTime, node->mChildren[i], globalTransform);
        }
}

const aiNodeAnim* Animator::FindNodeAnim(const aiAnimation* animation, const std::string& nodeName)
{
    for(unsigned int i = 0; i < animation->mNumChannels; i++) {
        aiNodeAnim* nodeAnim = animation->mChannels[i];
        if(std::string(nodeAnim->mNodeName.data) == nodeName) {
            return nodeAnim;
        }
    }
    return nullptr;
}

aiVector3D Animator::CalcInterpolatedPosition(float animationTime, const aiNodeAnim* nodeAnim)
{
    if(nodeAnim->mNumPositionKeys == 1) {
        return nodeAnim->mPositionKeys[0].mValue;
    }
        
    unsigned int positionIndex = FindPosition(animationTime, nodeAnim);
    unsigned int nextPositionIndex = positionIndex + 1;
    float deltaTime = nodeAnim->mPositionKeys[nextPositionIndex].mTime - nodeAnim->mPositionKeys[positionIndex].mTime;
    float factor = (animationTime - nodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;
        
    const aiVector3D& start = nodeAnim->mPositionKeys[positionIndex].mValue;
    const aiVector3D& end = nodeAnim->mPositionKeys[nextPositionIndex].mValue;
    aiVector3D delta = end - start;
        
    return start + factor * delta;
}

aiQuaternion Animator::CalcInterpolatedRotation(float animationTime, const aiNodeAnim* nodeAnim)
{
    if(nodeAnim->mNumRotationKeys == 1) {
        return nodeAnim->mRotationKeys[0].mValue;
    }
        
    unsigned int rotationIndex = FindRotation(animationTime, nodeAnim);
    unsigned int nextRotationIndex = rotationIndex + 1;
    float deltaTime = nodeAnim->mRotationKeys[nextRotationIndex].mTime - nodeAnim->mRotationKeys[rotationIndex].mTime;
    float factor = (animationTime - nodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;
        
    const aiQuaternion& start = nodeAnim->mRotationKeys[rotationIndex].mValue;
    const aiQuaternion& end = nodeAnim->mRotationKeys[nextRotationIndex].mValue;
        
    aiQuaternion result;
    aiQuaternion::Interpolate(result, start, end, factor);
    return result.Normalize();
}

aiVector3D Animator::CalcInterpolatedScaling(float animationTime, const aiNodeAnim* nodeAnim)
{
    if(nodeAnim->mNumScalingKeys == 1) {
        return nodeAnim->mScalingKeys[0].mValue;
    }
        
    unsigned int scalingIndex = FindScaling(animationTime, nodeAnim);
    unsigned int nextScalingIndex = scalingIndex + 1;
    float deltaTime = nodeAnim->mScalingKeys[nextScalingIndex].mTime - nodeAnim->mScalingKeys[scalingIndex].mTime;
    float factor = (animationTime - nodeAnim->mScalingKeys[scalingIndex].mTime) / deltaTime;
        
    const aiVector3D& start = nodeAnim->mScalingKeys[scalingIndex].mValue;
    const aiVector3D& end = nodeAnim->mScalingKeys[nextScalingIndex].mValue;
    aiVector3D delta = end - start;
        
    return start + factor * delta;
}

unsigned int Animator::FindPosition(float animationTime, const aiNodeAnim* nodeAnim)
{
    for(unsigned int i = 0; i < nodeAnim->mNumPositionKeys - 1; i++) {
        if(animationTime < nodeAnim->mPositionKeys[i+1].mTime) {
            return i;
        }
    }
    return 0;
}

unsigned int Animator::FindRotation(float animationTime, const aiNodeAnim* nodeAnim)
{
    for(unsigned int i = 0; i < nodeAnim->mNumRotationKeys - 1; i++) {
        if(animationTime < nodeAnim->mRotationKeys[i+1].mTime) {
            return i;
        }
    }
    return 0;
}

unsigned int Animator::FindScaling(float animationTime, const aiNodeAnim* nodeAnim)
{
    for(unsigned int i = 0; i < nodeAnim->mNumScalingKeys - 1; i++) {
        if(animationTime < nodeAnim->mScalingKeys[i+1].mTime) {
            return i;
        }
    }
    return 0;
}