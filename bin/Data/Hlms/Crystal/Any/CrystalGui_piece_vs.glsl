@property( crystal_gui )

@piece( custom_vs_attributes )
	in float4 normal;

	@property( crystal_text )
		in uint tangent;
		in uint2 blendIndices;
	@end
@end

@piece( custom_vs_preExecution )
	@property( !crystal_text )
		uint crystalDrawId = drawId + ((uint(gl_VertexID) - instance.worldMaterialIdx[drawId].w) / 54u);
	@end @property( crystal_text )
		uint crystalDrawId = drawId + ((uint(gl_VertexID) - instance.worldMaterialIdx[drawId].w) / 6u);
	@end
	#undef finalDrawId
	#define finalDrawId crystalDrawId
	#define worldViewProj 1.0f

	gl_ClipDistance[0] = normal.x;
	gl_ClipDistance[1] = normal.y;
	gl_ClipDistance[2] = normal.z;
	gl_ClipDistance[3] = normal.w;

	@property( crystal_text )
		uint vertId = (uint(gl_VertexID) - instance.worldMaterialIdx[drawId].w) % 6u;
		outVs.uvText.x = (vertId <= 1u || vertId == 5u) ? 0.0f : float( blendIndices.x );
		outVs.uvText.y = (vertId == 0u || vertId >= 4u) ? 0.0f : float( blendIndices.y );
		outVs.pixelsPerRow		= blendIndices.x;
		outVs.glyphOffsetStart	= tangent;
	@end
@end

@end