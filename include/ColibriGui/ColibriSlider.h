
#pragma once

#include "ColibriGui/ColibriWidget.h"

#include "OgreId.h"

namespace Ogre
{
	class HlmsUnlitDatablock;
}

COLIBRIGUI_ASSUME_NONNULL_BEGIN

namespace Colibri
{
	/** @ingroup Controls
	@class Slider
		Displays an interactive slider widget using two Renderables

		Layer 0 is drawn behind layer 1
	*/
	class Slider colibri_final : public Widget, Ogre::IdObject
	{
	protected:
		Renderable *colibrigui_nullable m_layers[2];

		float m_sliderValue;
		/// When directional actions (keyboard buttons) are applied,
		/// this is how much the slider value be increased or decreased.
		float m_directionChangeAmount;

		float m_cursorOffset;
		float m_handleSize;

	protected:
		void updateSlider();

	public:
		Slider( ColibriManager *manager );

		virtual void _initialize();
		virtual void _destroy();

		/// @copydoc Renderable::setVisualsEnabled
		void         setVisualsEnabled( bool bEnabled );
		virtual bool isVisualsEnabled() const colibri_final;

		virtual void setState( States::States state, bool smartHighlight = true,
							   bool broadcastEnable = false );

		// Set the value of the slider. Right now this is between 0 and 1 only.
		void  setValue( float value );
		float getValue() const { return m_sliderValue; }

		Renderable *getSliderLine();
		Renderable *getSliderHandle();

		virtual void setTransformDirty( uint32_t dirtyReason ) colibri_final;

		virtual void notifyCursorMoved( const Ogre::Vector2 &posNDC );
		virtual void _notifyActionKeyMovement( Borders::Borders direction );

		void  setDirectionChangeAmount( float amount ) { m_directionChangeAmount = amount; }
		float getDirectionChangeAmount() const { return m_directionChangeAmount; }

	private:
		void _processCursorPosition( const Ogre::Vector2 &pos, bool cursorBegin = false );
	};
}  // namespace Colibri

COLIBRIGUI_ASSUME_NONNULL_END