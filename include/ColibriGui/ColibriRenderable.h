
#pragma once

#include "ColibriGui/ColibriWidget.h"
#include "ColibriGui/Ogre/ColibriOgreRenderable.h"

#include "OgreColourValue.h"

namespace Ogre
{
	struct CbDrawCallStrip;
	struct CbDrawStrip;
	class HlmsColibri;
}

COLIBRIGUI_ASSUME_NONNULL_BEGIN

namespace Colibri
{
	struct StateInformation
	{
		//One set of UVs for each cell in the 3x3 grid.
		Ogre::Vector4	uvTopLeftBottomRight[GridLocations::NumGridLocations];
		float			borderSize[Borders::NumBorders];
		float			borderRepeatSize[Borders::NumBorders]; /// 0 or negative means disable repeat
		Ogre::IdString	materialName;
	};

	struct UiVertex
	{
		float x;
		float y;
		uint16_t u;
		uint16_t v;
		uint8_t rgbaColour[4];
		float clipDistance[Borders::NumBorders];
	};

	struct GlyphVertex
	{
		float x;
		float y;
		uint16_t width;
		uint16_t height;
		uint32_t offset;
		uint32_t rgbaColour;
		float clipDistance[Borders::NumBorders];
	};

	/** @ingroup Api_Backend
	@class ApiEncapsulatedObjects
		This structure encapsulates API-specific pointers required for rendering.
		Most or all of the classes inside this structure are forward declared to
		isolate ColibriGui's code from knowing about the 3D API backend as much as
		possible

		It's an argument to Renderable::_addCommands. Whatever happens inside
		Renderable::_addCommands will obviously be very API-specific.
	*/
	struct ApiEncapsulatedObjects
	{
		//Ogre::HlmsColibriGui		*hlms;
		Ogre::HlmsCache const		*lastHlmsCache;
		Ogre::HlmsCache const 		*passCache;
		Ogre::HlmsColibri			*hlms;
		Ogre::uint32				lastVaoName;
		Ogre::CommandBuffer			*commandBuffer;
		Ogre::IndirectBufferPacked	*indirectBuffer;
		uint8_t						*indirectDraw;
		uint8_t						*startIndirectDraw;
		//Used to see if we need to switch to a new draw when rendering text (since text rendering
		//has arbitrary number of of vertices, thus we can't properly calculate the drawId and
		//therefore the material ID)
		Ogre::HlmsDatablock			*lastDatablock;
		int							baseInstanceAndIndirectBuffers;
		Ogre::CbDrawCallStrip		* colibrigui_nullable drawCmd;
		Ogre::CbDrawStrip			* colibrigui_nullable drawCountPtr;
		uint16_t primCount;
		uint32_t accumPrimCount[2]; //[0] = regular widgets, [1] = text
	};

	/**
	@class Renderable
		Renderable must derive from Ogre::ColibriOgreRenderable (or encapsulated class)
		We can only share Vaos & Vertex Buffers, which will be owned by Window.
	*/
	class Renderable : public Widget, public Ogre::ColibriOgreRenderable
	{
	protected:
		StateInformation m_stateInformation[States::NumStates];

		Ogre::ColourValue	m_colour;

		/// WARNING: Most of the code assumes m_numVertices is hardcoded to 6*9;
		/// this value is dynamic because certain widgets (such as Labels) can
		/// have arbitrary number of vertices and the rest of the code
		/// also acknowledges that!
		uint32_t			m_numVertices;

	public:
		void _addCommands( ApiEncapsulatedObjects &apiObject );

	protected:
		inline void addQuad( UiVertex * RESTRICT_ALIAS vertexBuffer,
							 Ogre::Vector2 topLeft,
							 Ogre::Vector2 bottomRight,
							 Ogre::Vector4 uvTopLeftBottomRight,
							 uint8_t *rgbaColour,
							 Ogre::Vector2 parentDerivedTL,
							 Ogre::Vector2 parentDerivedBR,
							 Ogre::Vector2 invSize );

		virtual void stateChanged( States::States newState );

	public:
		Renderable( ColibriManager *manager );

		void setSkin( Ogre::IdString skinName, States::States forState );

		void setSkinPack( Ogre::IdString skinPackName );

		/** Directly set the skins via pointers instead of requiring map lookups.
		@param skinInfo
			Must not be null.
			skinInfo[i] can be null
			skinInfo must be able to hold States::NumStates elements
		*/
		void _setSkinPack( SkinInfo const * colibrigui_nonnull const * colibrigui_nullable skinInfos );

		virtual void setState( States::States state, bool smartHighlight=true,
							   bool broadcastEnable=false );

		/** Calls setClipBorders and makes the clipping region to match that of the current skin

			IMPORTANT: Skins' border size is dependent on real resolution (not canvas), thus
			this function should be called again and widgets be repositioned (and maybe even resized)
			if you change the real resolution.
		*/
		void setClipBordersMatchSkin();
		void setClipBordersMatchSkin( States::States state );

		virtual void broadcastNewVao( Ogre::VertexArrayObject *vao, Ogre::VertexArrayObject *textVao );

		virtual bool isRenderable() const	{ return true; }

		inline void _fillBuffersAndCommands( UiVertex * colibrigui_nonnull * colibrigui_nonnull
											 RESTRICT_ALIAS vertexBuffer,
											 GlyphVertex * colibrigui_nonnull * colibrigui_nonnull
											 RESTRICT_ALIAS textVertBuffer,
											 const Ogre::Vector2 &parentPos,
											 const Ogre::Vector2 &parentCurrentScrollPos,
											 const Ogre::Matrix3 &parentRot,
											 const Ogre::Vector2 &currentScrollPos,
											 bool forWindows );
		virtual void _fillBuffersAndCommands( UiVertex * colibrigui_nonnull * colibrigui_nonnull
											  RESTRICT_ALIAS vertexBuffer,
											  GlyphVertex * colibrigui_nonnull * colibrigui_nonnull
											  RESTRICT_ALIAS textVertBuffer,
											  const Ogre::Vector2 &parentPos,
											  const Ogre::Vector2 &parentCurrentScrollPos,
											  const Ogre::Matrix3 &parentRot );
	};
}

COLIBRIGUI_ASSUME_NONNULL_END