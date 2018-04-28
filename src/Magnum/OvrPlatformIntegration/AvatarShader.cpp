#include "AvatarShader.h"

#include <Magnum/Version.h>
#include <Magnum/Shader.h>

namespace Magnum { namespace OvrIntegration { namespace Platform {

AvatarShader::AvatarShader() {
    Shader vert{Version::GL330, Shader::Type::Vertex};
    Shader frag{Version::GL330, Shader::Type::Fragment};

    vert.addFile("AvatarShader.vert");
    frag.addFile("AvatarShader.frag");

    CORRADE_INTERNAL_ASSERT_OUTPUT(Shader::compile({vert, frag}));
    attachShaders({vert, frag});
    CORRADE_INTERNAL_ASSERT_OUTPUT(link());

    uniformViewPos = uniformLocation("viewPos");
    uniformWorldMat = uniformLocation("world");
    uniformViewProjMat = uniformLocation("viewProj");
    uniformMeshPose = uniformLocation("meshPose");
}

}}}
