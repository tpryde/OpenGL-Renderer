
varying vec3 v_Color;

void main()
{
    //gl_FragColor = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
	gl_FragColor = vec4( v_Color, 1.0 );
}
