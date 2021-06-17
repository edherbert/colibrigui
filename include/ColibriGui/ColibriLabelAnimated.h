
#pragma once

#include "ColibriGui/ColibriLabel.h"

COLIBRIGUI_ASSUME_NONNULL_BEGIN

namespace Colibri
{

	/** @ingroup Controls
	@class LabelAnimated
	*/
	class LabelAnimated : public Label
	{
	protected:

		struct GlyphAnimationValues
		{
			uint32_t rgbaColour;
			float xOffset;
			float yOffset;
		};
		typedef std::vector<GlyphAnimationValues> GlyphAnimationValueVec;

		GlyphAnimationValueVec m_animData[States::NumStates];

		virtual void addQuad( GlyphVertex * RESTRICT_ALIAS vertexBuffer,
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
								Matrix2x3 derivedRot );

	public:
		LabelAnimated( ColibriManager *manager );

		virtual void updateGlyphs( States::States state, bool bPlaceGlyphs );
	};
}

COLIBRIGUI_ASSUME_NONNULL_END
