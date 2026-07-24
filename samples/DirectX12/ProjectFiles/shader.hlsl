struct PSInput {
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

cbuffer cb0 : register(b0) {
    uint angle;
}

// Vertex Shader
PSInput VSMain(unsigned int index : SV_VertexID) {
    PSInput output;
    
    static float2 positions[3] = {
        float2(-0.43f, -0.25f),
	    float2(0.0f, 0.5f),
	    float2(0.43f, -0.25f)
    };
    
    static float3 colors[3] = {
        float3(1.0f, 0.0f, 0.0f),
	    float3(0.0f, 1.0f, 0.0f),
	    float3(0.0f, 0.0f, 1.0f)
    };
    
    float2 input_pos = positions[index];
    
    // Compute the rotation matrix
    float rotation_speed = -0.01f;
    
    float cosTheta = cos(angle * rotation_speed);
    float sinTheta = sin(angle * rotation_speed);
    
    float2 rotated_pos;
    rotated_pos.x = input_pos.x * cosTheta - input_pos.y * sinTheta;
    rotated_pos.y = input_pos.x * sinTheta + input_pos.y * cosTheta;
    
    output.position = float4(rotated_pos.x, rotated_pos.y, 0.0f, 1.0f);
    output.color = colors[index];
    return output;
}

// Pixel Shader
float4 PSMain(PSInput input) : SV_TARGET {
    return float4(input.color.r, input.color.g, input.color.b, 1.0);
}
