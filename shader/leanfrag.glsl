#version 450 core
in vec2 TexCoord_FS_in;

uniform sampler2D colortex;
uniform sampler2D bumptex;
uniform sampler2D bumpvartex;
uniform vec3 lightdir; 
uniform vec3 cameraPos;

out vec4 color;

void main()
{
   vec3 view = normalize(cameraPos - WorldPos);
   vec3 halfv = normalize(lightdir + view);
   
   color = texture2D(colortex, TexCoord_FS_in);

   float diffuse = dot(Normal, lightdir);

   float specular = pow(clamp( NdotH , 0.0, 1.0), 25.0);
   
   vec3 ret = diffuse * (color.xyz + vec3(0.5, 0.5, 0.5) * specular);

   color = vec4(ret, 1.0);
}