#version 330 core

in vec3 vertexWorldPos;
in vec3 vertexViewDir;
in vec3 vertexObjPos;
in vec3 vertexNormal;
in vec3 vertexTangent;
in vec3 vertexBitangent;
in vec2 vertexUV;

out vec4 fragmentColor;

void main() {
    vec3 worldNormal = normalize(vertexNormal);
    mat3 tangentTransform = mat3(vertexTangent, vertexBitangent, worldNormal);
    vec2 uv = vertexUV;
    vec4 color = baseColor;
    fragmentColor = color;
}
