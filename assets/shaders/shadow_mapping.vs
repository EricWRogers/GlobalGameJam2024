#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aBoneIDs;
layout (location = 4) in vec4 aWeights;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 MODEL;
uniform mat4 lightSpaceMatrix;
uniform mat4 boneTransforms[32];
uniform bool USEBONES;

void main()
{
    if (USEBONES)
    {
        mat4 boneTransform =
            aWeights[0] * boneTransforms[int(aBoneIDs[0])] +
            aWeights[1] * boneTransforms[int(aBoneIDs[1])] +
            aWeights[2] * boneTransforms[int(aBoneIDs[2])] +
            aWeights[3] * boneTransforms[int(aBoneIDs[3])];
        
        vs_out.FragPos = vec3(MODEL * boneTransform * vec4(aPos, 1.0));
        gl_Position = projection * view * MODEL * boneTransform * vec4(aPos, 1.0);
    
    }
    else
    {
        vs_out.FragPos = vec3(MODEL * vec4(aPos, 1.0));
        gl_Position = projection * view * MODEL * vec4(aPos, 1.0);
    }

    vs_out.Normal = transpose(inverse(mat3(MODEL))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
}
