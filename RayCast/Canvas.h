#pragma once

#include<cstdint>

namespace rc {

	/** Interface that hides the real graphics API and allows to replace it with a mock. 
	
	Not worried about the indirection performance cost. Testing is more important.
	If profiling shows this is a bottleneck, sub-classing can be replaced by templates.
	*/
	class Canvas {
	public:
		virtual ~Canvas() {};

		/** This is the only primitive operation required to draw the walls. 
		
		The slice is a 1-pixel-wide strip, to be drawn at the given column of the window.
		The strip starts at the top_row and goes down for all its height. The content is whatever is in the
		1-pixel-wide column of the wall texture at the given offset.

		In other words: take a column of pixels from the texture and copy it in the column of the screen between the 
		top row and the top row+height (scale it if necessary).

		Notice that the top row may be negative, the slice may start outside the screen when the player is very close
		to a wall.
		*/
		virtual void draw_wall_slice(const uint16_t column, const int16_t top_row, const uint16_t height, const uint16_t texture_offset) = 0;
	};
}