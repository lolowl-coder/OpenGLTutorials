#version 310 es

layout (local_size_x = 1) in;
layout (binding = 0, rgba8) writeonly uniform highp image2D outputImage;

struct ImageInfo
{
   ivec2 size;
   ivec2 srcOffset;
   ivec2 dstOffset;
};

layout(std430, binding = 0) buffer Buf0
{
   ImageInfo imageInfos[];
};

layout(std430, binding = 1) buffer Buf1
{
   uint flatImageData0[];
};

void main(void)
{
	uint imageInfoIndex = gl_GlobalInvocationID.x;
	ImageInfo imageInfo = imageInfos[imageInfoIndex];
	
	for(int y = 0; y < imageInfo.size.y; y++)
	{
		for(int x = 0; x < imageInfo.size.x; x++)
		{
			int flatIndex = imageInfo.srcOffset.x + y * imageInfo.size.x + x;
			uint uintTexel = flatImageData0[flatIndex];
			vec4 texel = unpackUnorm4x8(uintTexel);
			imageStore(outputImage, imageInfo.dstOffset + ivec2(x, y), texel);
		}
	}
}