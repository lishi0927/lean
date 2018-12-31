#version 450 core
in vec2 TexCoord_FS_in;
in vec3 WorldPos;

uniform sampler2D colortex;
uniform sampler2D bumptex;
uniform sampler2D bumpvartex;
uniform vec3 lightdir; 
uniform vec3 cameraPos;
uniform float sc;

out vec4 color;

const float texturewidth = 1024;
const float textureheight = 1024;
const float M_PI = 3.14159265358979323846;
const float specular_pow = 100.0;

const float gamma = 2.2;

vec3 pow_vec3(vec3 v, float p)
{
    return vec3(pow(v.x, p), pow(v.y, p), pow(v.z, p));
}
vec3 ToLinear(vec3 v) { return pow_vec3(v,     gamma); }
vec3 ToSRGB(vec3 v)   { return pow_vec3(v, 1.0/gamma); }

void main()
{   
   vec2 dx = dFdx(TexCoord_FS_in * vec2(texturewidth, textureheight));
   vec2 dy = dFdy(TexCoord_FS_in * vec2(texturewidth, textureheight));
   float d = max( dot( dx, dx ), dot( dy, dy ) );
   
   d = clamp(sqrt(d), 1.0, 128.0);
   float mipLevel = log2(d);
   mipLevel = floor(mipLevel);

   vec2 bump = textureLod(bumptex, TexCoord_FS_in, mipLevel).rg;
   vec3 bump_var = textureLod(bumpvartex, TexCoord_FS_in, mipLevel).rgb;

   bump = (bump * 2.0 - 1.0) * sc;
   bump_var.z = (bump_var.z * 2.0 - 1.0) * sc * sc;
   bump_var.xy = bump_var.xy * sc * sc + 1.0 / specular_pow;

   vec3 Normal = normalize(vec3(bump, 1.0));

   vec3 view = normalize(cameraPos - WorldPos);
   vec3 halfv = normalize(lightdir + view);

   vec3 sigma = bump_var - vec3(bump.x * bump.x, bump.y * bump.y, bump.x * bump.y);
   float Det = sigma.x * sigma.y - sigma.z * sigma.z;

   vec2 halfc = halfv.xy / halfv.z - bump;
   float e = (halfc.x * halfc.x * sigma.y + halfc.y * halfc.y * sigma.x - 2.0 * halfc.x * halfc.y * sigma.z);

   color = texture2D(colortex, TexCoord_FS_in);

   float diffuse = clamp(dot(Normal, lightdir), 0.0, 1.0);

   float specular = (Det <=0) ? 0: exp(-0.5 * e / Det) / sqrt(Det) / 2 / M_PI;
   
   vec3 cd = ToLinear(vec3(0.5, 0.13,0.1));
   vec3 cs = ToLinear(vec3(0.57, 0.57, 0.57));
   
   vec3 ret = diffuse * (cd + cs * specular);
    ret = ToSRGB(ret);

   color = vec4(ret, 1.0);
}