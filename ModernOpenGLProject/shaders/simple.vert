
attribute vec2 a_Position;
attribute vec3 a_Color;
uniform float u_Offset; // Offset. 
varying vec3 v_Color;
uniform float u_Rotation;

void main()
{
	mat2 rotation;
	
	rotation[0][0] = cos(u_Rotation);
	rotation[0][1] = sin(u_Rotation);
	rotation[1][0] = -sin(u_Rotation);
	rotation[1][1] = cos(u_Rotation);
	
    gl_Position = vec4( a_Position * rotation, 0.0, 1.0 );

	v_Color = a_Color;
}
