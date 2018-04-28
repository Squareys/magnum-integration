#ifndef Magnum_OvrPlatformIntegration_AvatarShader_h_
#define Magnum_OvrPlatformIntegration_AvatarShader_h_

#include <Magnum/AbstractShaderProgram.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Matrix4.h>

namespace Magnum { namespace OvrIntegration { namespace Platform {

class AvatarShader: public AbstractShaderProgram {
    public:
        typedef Attribute<0, Vector3> Position;
        typedef Attribute<1, Vector3> Normal;
        typedef Attribute<2, Vector4> Tangent;
        typedef Attribute<3, Vector2> TextureCoord;
        typedef Attribute<4, Vector4> BlendIndices;
        typedef Attribute<5, Vector4> BlendWeights;

        AvatarShader();

        void setViewPosition(const Vector3& pos) {
            setUniform(uniformViewPos, pos);
        }

        void setWorldMatrix(const Matrix4& mat) {
            setUniform(uniformWorldMat, mat);
        }

        void setViewProjectionMatrix(const Matrix4& mat) {
            setUniform(uniformViewProjMat, mat);
        }

        void setMeshPoses(const Containers::ArrayView<Matrix4> poses) {
            setUniform(uniformMeshPose, poses);
        }

    private:
        Int uniformViewPos,
            uniformWorldMat,
            uniformViewProjMat,
            uniformMeshPose;
};

}}}

#endif
