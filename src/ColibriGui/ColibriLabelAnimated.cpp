
#include "ColibriGui/ColibriLabelAnimated.h"

namespace Colibri
{

	LabelAnimated::LabelAnimated( ColibriManager *manager ) :
		Label( manager )
	{

	}

	void LabelAnimated::addQuad( GlyphVertex * RESTRICT_ALIAS vertexBuffer,
								uint32_t glyphId,
								States::States currentState,
								Ogre::Vector2 topLeft,
								Ogre::Vector2 bottomRight,
								uint16_t glyphWidth,
								uint16_t glyphHeight,
								uint32_t rgbaColour,
								Ogre::Vector2 parentDerivedTL,
								Ogre::Vector2 parentDerivedBR,
								Ogre::Vector2 invSize,
								uint32_t offset,
								float canvasAspectRatio,
								float invCanvasAspectRatio,
								Matrix2x3 derivedRot )
	{
		Ogre::Vector2 tmp2d;

		const GlyphAnimationValues& animData = m_animData[currentState][glyphId];

		uint32_t finalColour = rgbaColour + animData.rgbaColour;
		topLeft += Ogre::Vector2(animData.xOffset, animData.yOffset);
		bottomRight += Ogre::Vector2(animData.xOffset, animData.yOffset);

		#define COLIBRI_ADD_VERTEX( _x, _y, _u, _v, clipDistanceTop, clipDistanceLeft, \
									clipDistanceRight, clipDistanceBottom ) \
			tmp2d = Widget::mul( derivedRot, _x, _y * invCanvasAspectRatio ); \
			tmp2d.y *= canvasAspectRatio; \
			vertexBuffer->x = tmp2d.x; \
			vertexBuffer->y = -tmp2d.y; \
			vertexBuffer->width = glyphWidth; \
			vertexBuffer->height = glyphHeight; \
			vertexBuffer->offset = offset;\
			vertexBuffer->rgbaColour = finalColour; \
			vertexBuffer->clipDistance[Borders::Top]	= clipDistanceTop; \
			vertexBuffer->clipDistance[Borders::Left]	= clipDistanceLeft; \
			vertexBuffer->clipDistance[Borders::Right]	= clipDistanceRight; \
			vertexBuffer->clipDistance[Borders::Bottom]	= clipDistanceBottom; \
			++vertexBuffer

		COLIBRI_ADD_VERTEX( topLeft.x, topLeft.y,
							0u, 0u,
							(topLeft.y - parentDerivedTL.y) * invSize.y,
							(topLeft.x - parentDerivedTL.x) * invSize.x,
							(parentDerivedBR.x - topLeft.x) * invSize.x,
							(parentDerivedBR.y - topLeft.y) * invSize.y );

		COLIBRI_ADD_VERTEX( topLeft.x, bottomRight.y,
							0u, glyphHeight,
							(bottomRight.y - parentDerivedTL.y) * invSize.y,
							(topLeft.x - parentDerivedTL.x) * invSize.x,
							(parentDerivedBR.x - topLeft.x) * invSize.x,
							(parentDerivedBR.y - bottomRight.y) * invSize.y );

		COLIBRI_ADD_VERTEX( bottomRight.x, bottomRight.y,
							glyphWidth, glyphHeight,
							(bottomRight.y - parentDerivedTL.y) * invSize.y,
							(bottomRight.x - parentDerivedTL.x) * invSize.x,
							(parentDerivedBR.x - bottomRight.x) * invSize.x,
							(parentDerivedBR.y - bottomRight.y) * invSize.y );

		COLIBRI_ADD_VERTEX( bottomRight.x, bottomRight.y,
							glyphWidth, glyphHeight,
							(bottomRight.y - parentDerivedTL.y) * invSize.y,
							(bottomRight.x - parentDerivedTL.x) * invSize.x,
							(parentDerivedBR.x - bottomRight.x) * invSize.x,
							(parentDerivedBR.y - bottomRight.y) * invSize.y );

		COLIBRI_ADD_VERTEX( bottomRight.x, topLeft.y,
							glyphWidth, 0u,
							(topLeft.y - parentDerivedTL.y) * invSize.y,
							(bottomRight.x - parentDerivedTL.x) * invSize.x,
							(parentDerivedBR.x - bottomRight.x) * invSize.x,
							(parentDerivedBR.y - topLeft.y) * invSize.y );

		COLIBRI_ADD_VERTEX( topLeft.x, topLeft.y,
							0u, 0u,
							(topLeft.y - parentDerivedTL.y) * invSize.y,
							(topLeft.x - parentDerivedTL.x) * invSize.x,
							(parentDerivedBR.x - topLeft.x) * invSize.x,
							(parentDerivedBR.y - topLeft.y) * invSize.y );

		#undef COLIBRI_ADD_VERTEX
	}
	//-------------------------------------------------------------------------
	void LabelAnimated::updateGlyphs( States::States state, bool bPlaceGlyphs )
	{
		Label::updateGlyphs( state, bPlaceGlyphs );

		for(uint8_t i = 0; i < States::NumStates; i++)
		{

			m_animData[i].resize(m_shapes[i].size());

			for(uint32_t y = 0; y < m_shapes[i].size(); y++)
			{
				m_animData[i][y].rgbaColour =
					y % 2 == 0 ? Ogre::ColourValue::Red.getAsRGBA() : Ogre::ColourValue::White.getAsRGBA();
				m_animData[i][y].xOffset = 0.01;
				m_animData[i][y].yOffset = 0.01 * y;
			}


		}
	}

}
