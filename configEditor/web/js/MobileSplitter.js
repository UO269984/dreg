import {isTouchScreen, setWindowSelect} from "./graph/CanvasManager.js"

class MobileSplitterBase {
	constructor(splitter, toStretchLeft, toStretchRight) {
		this.pressed = false
		this.toStretchLeft = toStretchLeft
		this.toStretchRight = toStretchRight
		
		this.splitterWidth_2 = splitter.getBoundingClientRect().width / 2
		//Center the splitter
		this.pressedAt((this.toStretchLeft.getBoundingClientRect().width +
			toStretchRight.getBoundingClientRect().width + this.splitterWidth_2) / 2)
	}
	pressedAt(x) {
		this.toStretchLeft.style.width = (x - this.splitterWidth_2) + "px"
		this.toStretchRight.style.width = (window.innerWidth - x - this.splitterWidth_2) + "px"
	}
}

class MouseMobileSplitter extends MobileSplitterBase {
	constructor(splitter, toStretchLeft, toStretchRight) {
		super(splitter, toStretchLeft, toStretchRight)
		
		splitter.addEventListener("mousedown", this.setPressedEvent.bind(this, true))
		document.addEventListener("mouseup", this.setPressedEvent.bind(this, false))
		document.addEventListener("mousemove", e => {
			if (this.pressed)
				this.pressedAt(e.pageX)
		})
	}
	setPressedEvent(newPressed, event) {
		if (event.button == 0 && this.pressed != newPressed) {
			this.pressed = newPressed
			setWindowSelect(! this.pressed)
		}
	}
}

class TouchMobileSplitter extends MobileSplitterBase {
	constructor(splitter, toStretchLeft, toStretchRight) {
		super(splitter, toStretchLeft, toStretchRight)
		
		splitter.addEventListener("touchstart", e => this.pressed = true)
		document.addEventListener("touchend", e => this.pressed = false)
		document.addEventListener("touchmove", e => {
			if (this.pressed)
				this.pressedAt(e.touches[0].pageX)
		})
	}
}

export const MobileSplitter = isTouchScreen() ? TouchMobileSplitter : MouseMobileSplitter