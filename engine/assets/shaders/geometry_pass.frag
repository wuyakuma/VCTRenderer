#version 430

layout(location = 0) out vec3 gNormal;
layout(location = 1) out vec3 gAlbedo;
layout(location = 2) out vec4 gSpecular;

in Vertex
{
    vec3 position;
    vec3 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
};

uniform struct Material
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
    uint useNormalsMap;
} material;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalsMap;

uniform float alphaCutoff = 0.1;

vec3 normalMapping()
{
    vec3 norm = normalize(normal);
    vec3 tang = normalize(tangent);
    tang = normalize(tang - dot(tang, norm) * norm);
    vec3 bTan = normalize(bitangent);
    vec3 tNormal = texture(normalsMap, texCoord.xy).rgb;
    vec3 bumpedNormal = 2.0 * tNormal - vec3(1.0);
    mat3 TBN = mat3(tang, bTan, norm);
    return normalize(TBN * bumpedNormal);
}

void main()
{
    // store albedo rgb
    vec4 diffuseColor = texture(diffuseMap, texCoord.xy);

    if (diffuseColor.a <= alphaCutoff) { discard; }

    gAlbedo = diffuseColor.rgb * material.diffuse;
    // store specular intensity
    vec4 specularColor = texture(specularMap, texCoord.xy);
    gSpecular = vec4(specularColor.rgb * material.specular, material.shininess);
    // store per fragment normal
    gNormal = material.useNormalsMap == 1 ? normalMapping() : normalize(normal);
}