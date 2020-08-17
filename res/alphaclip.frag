varying vec2 fragTexCoord;
varying vec3 fragPosition;
varying vec3 fragNormal;
varying vec4 fragColor;

uniform sampler2D texture0;


void main()
{
	vec4 color = texture2D(texture0, fragTexCoord) * fragColor;
	if(color.a < 0.4) {discard;}
	gl_FragColor = color;
}
