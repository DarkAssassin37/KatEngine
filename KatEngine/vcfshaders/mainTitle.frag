float nonLin(float x)
{
    return x*x;
}

float gradCurve(float x)
{
    return pow(x,4.0);
}


vec2 windDisplace(vec2 uv, vec2 direction, float speed)
{
    float anim = iMouse.x / iResolution.x;
    float off = gradCurve(anim);
    float noise =  min(1.0, texture(iChannel3, uv).x + off*0.5);
    noise = 0.8 + noise*0.2;
    float windStr = min(1.0, texture(iChannel2, uv).x + off) * noise;

    windStr = gradCurve(windStr);
    

    float amount = nonLin(anim) * windStr * speed;
    
    return amount*direction;
}

vec2 winTransform(vec2 uv)
{
    vec2 windDir = vec2(0.7,0.1);
    windDir = normalize(windDir);
    float windSpeed = 1000.0;
    
    vec2 uvAbove = uv + vec2(0.0,1.0)/iResolution.xy;
        
    float sticky = length(windDisplace(uvAbove, windDir, windSpeed));
    
    return uv + windDisplace(uv, windDir, windSpeed) * sticky;
}

vec2 frostedGlassTransform(vec2 uv)
{
    vec2 mouse = iMouse.xy / iResolution.xy;
    
    vec2 normal = texture(iChannel1, uv).xy;
    float amount = length(vec2(0.5) - uv);
    amount = pow(amount, 4.0);
    vec2 currPos = vec2(0.0);
    currPos.xy = uv;
    currPos += (normal * 2.0 - 1.0) * mouse.x * 0.5 * amount;
	return currPos;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{

    vec2 mouse = iMouse.xy / iResolution.xy;
    vec2 uv = fragCoord/iResolution.xy;

    uv = winTransform(uv);
	uv = frostedGlassTransform(uv);

    fragColor = texture(iChannel0, uv);
}