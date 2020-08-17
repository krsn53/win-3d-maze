varying vec2 fragTexCoord;
varying vec3 fragPosition;
varying vec3 fragNormal;


void main()
{
	vec3 angle = normalize(vec3(0.0, 1.0, 1.0));
    gl_FragColor =vec4((vec3(dot(fragNormal, angle) + 1.0)/ 2.0), 1.0);
}
