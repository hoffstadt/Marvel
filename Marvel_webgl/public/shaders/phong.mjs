
export const vs_phong = `#version 300 es
            
            in vec4 coordinates;
            in vec4 normals;
            
            out vec3 viewPos;
            out vec3 viewNormal;
            out vec4 position;
            
            uniform mat4 uModelMatrix;
            uniform mat4 uModelViewMatrix;
            uniform mat4 uModelViewProjectionMatrix;
                      
            void main() {
              viewPos = vec3(uModelViewMatrix * vec4(coordinates[0], coordinates[1], coordinates[2], 1));
              viewNormal = mat3(uModelViewMatrix) * vec3(normals);
              position = uModelViewProjectionMatrix * coordinates;
              gl_Position = position;
            }
          `;

export const ps_phong =`#version 300 es
            precision highp float;
            
            in vec3 viewPos;
            in vec3 viewNormal;
            in vec4 position;
            
            out vec4 outputColor;
            
            uniform vec3 uPosition;
            uniform vec3 uAmbient;
            uniform vec3 uDiffuse;
            uniform float uDiffuseIntensity;
            uniform float uAttConst;
            uniform float uAttLin;
            uniform float uAttQuad;
            
            vec3 Speculate(
                const in vec3 specularColor,
                in float specularIntensity,
                const in vec3 iviewNormal,
                const in vec3 viewFragToL,
                const in vec3 iviewPos,
                const in float att,
                const in float specularPower)
            {
                // calculate reflected light vector
                vec3 w = iviewNormal * dot(viewFragToL, iviewNormal);
                vec3 r = normalize(w * 2.0f - viewFragToL);
                
                // vector from camera to fragment (in view space)
                vec3 viewCamToFrag = normalize(iviewPos);
                
                // calculate specular component color based on angle between
                // viewing vector and reflection vector, narrow with power function
                return att * specularColor * specularIntensity * pow(max(0.0f, dot(-r, viewCamToFrag)), specularPower);
            }
            
            vec3 Diffuse(
                vec3 diffuseColor,
                float diffuseIntensity,
                in float att,
                in vec3 viewDirFragToL,
                in vec3 viewNormal)
            {
                return diffuseColor * diffuseIntensity * att * max(0.0f, dot(viewDirFragToL, viewNormal));
            }
            
            float Attenuate(float attConst, float attLin, float attQuad, in float distFragToL)
            {
                return 1.0f / (attConst + attLin * distFragToL + attQuad * (distFragToL * distFragToL));
            }
            
            struct LightVectorData
            {
                vec3 vToL;
                vec3 dirToL;
                float distToL;
            };
            
            LightVectorData CalculateLightVectorData(in vec3 lightPos, in vec3 fragPos)
            {
                LightVectorData lv;
                lv.vToL = lightPos - fragPos;
                lv.distToL = length(lv.vToL);
                lv.dirToL = lv.vToL / lv.distToL;
                return lv;
            }
            
            void main() {
                
                vec3 diffuse;
                vec3 material = vec3(0.0, 1.0, 0.0);
                vec3 specularColor = vec3(1.0, 1.0, 1.0);
                float specularWeight = 1.0;
                float specularGloss = 15.0;
                
                vec3 nviewNormal = normalize(viewNormal);
                
                // fragment to light vector data
                vec3 pos = vec3(position[0], position[1], position[2]);
                LightVectorData lv = CalculateLightVectorData(uPosition, viewPos);
                
                // attenuation
                float att = Attenuate(uAttConst, uAttLin, uAttQuad, lv.distToL);
                
                // diffuse
                diffuse = Diffuse(uDiffuse, uDiffuseIntensity, att, lv.dirToL, nviewNormal);
                
                // specular
                vec3 specular = Speculate(
                        uDiffuse * uDiffuseIntensity * specularColor, specularWeight, nviewNormal,
                        lv.vToL, viewPos, att, specularGloss);
            
                // final color
                vec3 toclamp = clamp((diffuse + uAmbient) * material + specular, 0.0, 1.0);
                outputColor = vec4(toclamp[0], toclamp[1], toclamp[2], 1.0);
            }`;