
#version 330 core

uniform sampler2D tex;


in vec2 i_tc;
in vec3 n;
in vec4 FragPos;
in vec4 lightPos;
in vec3 viewPos;

out vec4 FragColor;


void main()
{
    vec4 lightColor_lp = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    vec4 lightColor_ld = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    vec4 norm_n = vec4(normalize(n),0.0f);              //znormalizowany wektor normalny
    vec4 viewDir = normalize(vec4(viewPos,1.0) - FragPos);

    //ambient 
        float wsp_ambient = 0.2;
        vec4 ambient = wsp_ambient * lightColor_ld;

    //diffuse  

        //dla punktowego
            float wsp_diffuse_lp = 0.09f;
            vec4 lightDir_lp = normalize(lightPos - FragPos);
            float diff_lp = max(dot(norm_n, lightDir_lp), 0.0);
            vec4 diffuse_lp = diff_lp * lightColor_lp*wsp_diffuse_lp;

        //dla kierunkowego        
            float wsp_diffuse_ld = 0.3f;
            vec4 lightDir_ld = normalize(vec4(-2.0f,3.0f,5.0f,0.0f));
            float diff_ld = wsp_diffuse_ld*max(dot(norm_n, lightDir_ld), 0.0);

    //specular

        //dla punktowego
            float wsp_specular_lp = 0.2f;
            vec4 reflectDir_lp = reflect(-lightDir_lp, norm_n); 
            float spec_lp = pow(max(dot(viewDir, reflectDir_lp), 0.0), 100);
            vec4 specular_lp = wsp_specular_lp * spec_lp * lightColor_lp; 

        //dla kierunkowego  
            float wsp_specular_ld = 0.8f;
            vec4 reflectDir_ld = reflect(-lightDir_ld, norm_n); 
            float spec_ld = pow(max(dot(viewDir, reflectDir_ld), 0.0), 100);
            vec4 specular_ld = wsp_specular_ld * spec_ld * lightColor_ld;


    vec4 result =(diffuse_lp+ambient+specular_lp+diff_ld+specular_ld) * vec4(texture(tex,i_tc).rgb,1.0); //
    FragColor = result;

}
