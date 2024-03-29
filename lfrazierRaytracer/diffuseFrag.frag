#version 150 

//these are the interpolated values out of the rasterizer, so you can't know
//their specific values without knowing the vertices that contributed to them
in vec3 fs_normal;
in vec3 fs_light_vector;
in vec3 fs_color;

out vec4 out_Color;

void main() {
    //base colors for materials
    vec4 diffuseColor = vec4(fs_color, 1.0);
	vec4 ambientColor = vec4(0.2*fs_color, 1.0);

    
    //calculate diffuse term and clamp to the range [0, 1]
    float diffuseTerm = clamp(dot(normalize(fs_normal), normalize(fs_light_vector)), 0.0, 1.0);

	//base: dot(normalized_reflected_light_ray, normalized_camera_position), clamp to 0
	//exp is in properties
    
    out_Color = ambientColor + diffuseColor * diffuseTerm;
}