#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;
uniform bool horizontal;
uniform float weight[8] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162
                                    , 0.0142162162, 0.0122162162, 0.0102162162);

float scaleOffset = 2.0;

void main()
{             
    vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
    vec2 tex_scale;
    vec4 baseImage = texture(image, TexCoords);
    vec3 result = baseImage.rgb * weight[0];
    if(horizontal)
    {
        for(int i = 1; i < 8; ++i)
        {
            tex_scale = vec2(tex_offset.x * i * scaleOffset, 0.0);
            result += texture(image, TexCoords + tex_scale).rgb * weight[i];
            result += texture(image, TexCoords - tex_scale).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 8; ++i)
        {
            tex_scale = vec2(0.0, tex_offset.y * i * scaleOffset);
            result += texture(image, TexCoords + tex_scale).rgb * weight[i];
            result += texture(image, TexCoords - tex_scale).rgb * weight[i];
        }
    }

    if (baseImage.r + baseImage.g + baseImage.b > result.r + result.g + result.b)
        FragColor = baseImage;
    else
        FragColor = vec4(result, baseImage.a);
}