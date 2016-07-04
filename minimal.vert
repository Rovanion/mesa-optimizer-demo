#version 130

in  vec3 in_Position;
in  vec3 in_Color;
out vec3 ex_Color;

void main(void) {
	// The below line can be optimized in the NIR through some tree rotations.
	ex_Color = 0.1 + in_Color + 0.2 - 0.3;
	gl_Position = vec4(in_Position, 1.0);
}
