
#pragma once

#include "ColibriGui/ColibriRenderable.h"

COLIBRIGUI_ASSUME_NONNULL_BEGIN

namespace Colibri
{
	/**
	@class Window
		Windows are special Widgets:
			1. They can only be children of other Windows
			2. m_parent can be nullptr
			3. Regular widgets can be navigated via left, up, down, etc.
			   Windows can only be navigated by entering or leaving them.
	*/
	class Window : public Renderable
	{
		friend class ColibriManager;

		Ogre::Vector2 m_currentScroll;
		/// For smooth scrolling, m_nextScroll contains the scroll destination,
		/// eventually over time m_currentScroll = m_nextScroll.
		Ogre::Vector2 m_nextScroll;
		Ogre::Vector2 m_scrollableArea;

		/// In range [0; m_windowsStart)
		uint16_t	m_defaultChildWidget;
		/// In range [0; m_windowsStart)
		/// When uint16_t::max, then we look in m_defaultChildWidget instead
		uint16_t	m_lastPrimaryAction;
		/// When true, all of our immediate children widgets are dirty and need to be recalculated
		bool		m_widgetNavigationDirty;
		/// When true, all of our immediate children windows are dirty and need to be recalculated
		bool		m_windowNavigationDirty;
		/// When true, all of our immediate children (widgets or windows)
		/// are not dirty, but one of our children's child is.
		bool		m_childrenNavigationDirty;

		WindowVec m_childWindows;

		void notifyChildWindowIsDirty();

		/// Overloaded to also reorder the m_childWindows vec.
		virtual void reorderWidgetVec( bool widgetInListDirty, WidgetVec& widgets ) colibri_override;

		Window* getParentAsWindow() const;

		virtual size_t notifyParentChildIsDestroyed( Widget *childWidgetBeingRemoved ) colibri_override;

	public:
		Window( ColibriManager *manager );
		virtual ~Window() colibri_override;

		virtual void _initialize() colibri_override;
		virtual void _destroy() colibri_override;
		virtual bool isWindow() const colibri_final	{ return true; }

		/** Smoothly scrolls from current location towards input destination.
		@param nextScroll
		@param animateOutOfRange
			When true, if the input is outside the range [0; m_maxScroll] this will be
			allowed and a small "bump" will be animated.
			When false, the input gets clamped and no bump is animated.
		*/
		void setScrollAnimated( const Ogre::Vector2 &nextScroll, bool animateOutOfRange );
		/// Immediately sets the scroll to the input destination.
		/// If the input is outside the range [0; m_maxScroll], it will be clamped.
		void setScrollImmediate( const Ogre::Vector2 &scroll );
		/// Return the current scroll position. Note that it may not be the final one,
		/// if it's still animating.
		/// To get the final scroll to achieve once animation finishes, use getNextScroll
		/// This value may be temporarily outside the range [0; m_maxScroll]
		virtual const Ogre::Vector2& getCurrentScroll() const colibri_final;
		/// Returns the final scroll position. See getCurrentScroll
		/// This value may be temporarily outside the range [0; m_maxScroll]
		const Ogre::Vector2& getNextScroll() const						{ return m_nextScroll; }

		/// Sets the maximum scroll setScrollImmediate/setScrollAnimated can go. 0 to disable scrolling
		/// The value is in canvas space.
		void setMaxScroll( const Ogre::Vector2 &maxScroll );

		/// Calculates how much scroll is needed based on the current size,
		/// the clipping borders, and the scrollable area
		Ogre::Vector2 getMaxScroll() const;

		void setScrollableArea( const Ogre::Vector2 &scrollableArea );
		const Ogre::Vector2& getScrollableArea() const;

		/// Returns true if getMaxScroll() is non-zero in any direction.
		bool hasScroll() const;

		/// Calculates & sets the required scrollable area based on the current size of all child
		/// widgets & windows; and our current size.
		/// This function will not call sizeToFit on children. You'll likely want to call this last.
		virtual void sizeScrollToFit() colibri_override;

		/// Returns true if it's still updating its scroll and the
		/// focused widget by the mouse cursor is potentially dirty
		bool update( float timeSinceLast );

		/// See Widget::setWidgetNavigationDirty
		/// Notifies all of our children widgets are dirty and we need to recalculate them.
		/// Also inform our parent windows they need to call us for recalculation
		virtual void setWidgetNavigationDirty() colibri_override;

		/// Similar to setWidgetNavigationDirty, but you should call this if this window
		/// has changed, and we'll inform our parent that it needs to recalculate
		/// all of its windows, and also broadcast to all parents they need to call our
		/// parent.
		void setWindowNavigationDirty();

		/// Reparents the given child window to this, 'this' becomes the parent
		void attachChild( Window *window );
		/// The given child window is detached; and becomes parentless (i.e. a main window)
		void detachChild( Window *window );
		/// Detaches from current parent. Does nothing if already parentless
		void detachFromParent();

		/// Makes this widget the default widget (i.e. which widget the cursor
		/// defaults to when the window is created)
		/// If widget is not our child or nullptr, the current default is unset
		void setDefault( Widget * colibrigui_nullable widget );
		Widget* colibrigui_nullable getDefaultWidget() const;

		/** Used to remember the last widget this window was into, so that user
			can call:

			@code
				Widget *rememberedWidget = window->getLastPrimaryAction();
				if( rememberedWidget )
					rememberedWidget->setKeyboardFocus();
			@endcode

			when returning to this window (e.g. because a child window got destroyed)
		@remarks
			When there is no remembered widget, getDefaultWidget is returned instead.
		*/
		void    setLastPrimaryAction( Widget *colibrigui_nullable widget );
		Widget *colibrigui_nullable getLastPrimaryAction() const;

		/** When true, clicking on an empty part of the window will consume mouse
			cursor movements and clicks

			When false, mouse events will "go through" as if the window wasn't there;
			unless they land on a child widget (or a child window that does consume
			the cursor)

			The default value is true.
		@param bConsumeCursor
		*/
		void setConsumeCursor( bool bConsumeCursor ) { m_clickable = bConsumeCursor; }
		bool getConsumeCursor() const { return m_clickable; }

		virtual void _updateDerivedTransformOnly( const Ogre::Vector2 &parentPos,
												  const Matrix2x3 &parentRot ) colibri_override;

		virtual void _fillBuffersAndCommands( UiVertex * colibrigui_nonnull * colibrigui_nonnull
											  RESTRICT_ALIAS vertexBuffer,
											  GlyphVertex * colibrigui_nonnull * colibrigui_nonnull
											  RESTRICT_ALIAS textVertBuffer,
											  const Ogre::Vector2 &parentPos,
											  const Ogre::Vector2 &parentCurrentScrollPos,
											  const Matrix2x3 &parentRot ) colibri_final;
	};
}

COLIBRIGUI_ASSUME_NONNULL_END
