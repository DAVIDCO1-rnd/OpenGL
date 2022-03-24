#version 330 core

#define A texture(iChannel0, v/iResolution.xy).r
void mainImage(out vec4 o, vec2 u) {
	vec2 v=u;
    o += 27.4*(.05-length(A-vec2( (v.x++,A),(v.x--,v.y++,A) )));}