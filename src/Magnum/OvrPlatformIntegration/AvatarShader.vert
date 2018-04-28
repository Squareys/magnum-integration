#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec2 texCoord;
layout (location = 4) in vec4 poseIndices;
layout (location = 5) in vec4 poseWeights;

out vec3 vertexWorldPos;
out vec3 vertexViewDir;
out vec3 vertexObjPos;
out vec3 vertexNormal;
out vec3 vertexTangent;
out vec3 vertexBitangent;
out vec2 vertexUV;

uniform vec3 viewPos;
uniform mat4 world;
uniform mat4 viewProj;
uniform mat4 meshPose[64];

void main() {
    vec4 vertexPose = vec(0.0, 0.0, 0.0, 0.0);
    vec4 normalPose = vec(0.0, 0.0, 0.0, 0.0);
    vec4 tangentPose = vec(0.0, 0.0, 0.0, 0.0);
    for(int i = 0; i < 4; ++i) {
        const float w = poseWeights[i];
        const mat4 p = meshPose[int(poseIndices[i])];
        vertexPose += p*vec4(w*position, w);
        normalPose += p*vec4(w*normal, 0.0);
        tangentPose += p*vec4(w*tangent.xyz, 0.0);
    }

    normalPose = normalize(normalPose);
    tangentPose = normalize(tangentPose);

    vertexWorldPos = vec3(world * vertexPose);
    gl_Position = viewProj * vec4(vertexWorldPos, 1.0);
    vertexViewDir = normalize(viewPos - vertexWorldPos.xyz);
    vertexObjPos = position.xyz;
    vertexNormal = (world * normalPose).xyz;
    vertexTangent = (world * tangentPose).xyz;
    vertexBitangent = normalize(cross(vertexNormal, vertexTangent) * tangent.w);
    vertexUV = texCoord;
}
