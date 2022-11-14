
class InputManager {
	constructor() {
		this.inputPrecision = 1
		this.canvasMap = new Map()
		this.activeCanvasFuncs = null
	}
	
	addCanvas(canvas, funcsObj) {
		this.canvasMap.set(canvas, funcsObj)
	}
	
	addCanvasAction(canvas, actionName, func) {
		this.canvasMap.get(canvas)[actionName] = func
	}
}

class MouseInputManager extends InputManager {
	constructor() {
		super()
		this.inputPrecision = 1
		this.mouseOverCanvasFuncs = null
		
		this.interactActive = false
		this.scaleActive = false
		
		document.onmousemove = this.#mouseMoveHandler.bind(this)
		document.onkeydown = e => {
			if (e.key == "Control")
				this.scaleActive = true
		}
		document.onkeyup = e => {
			if (e.key == "Control")
				this.scaleActive = false
		}
		document.onmousedown = e => {
			let isInteractBt = e.button == 0
			if ((isInteractBt || e.button == 1) && this.mouseOverCanvasFuncs != null) {
				this.activeCanvasFuncs = this.mouseOverCanvasFuncs
				setWindowSelect(false)
				
				this.interactActive = isInteractBt
				if (isInteractBt)
					this.activeCanvasFuncs.mouseDownCallback(e)
				
				return false
			}
		}
		document.onmouseup = e => {
			if ((e.button == 0 || e.button == 1) && this.activeCanvasFuncs != null) {
				this.activeCanvasFuncs.mouseUpCallback(e)
				this.activeCanvasFuncs = null
				setWindowSelect(true)
			}
		}
	}
	
	addCanvas(canvas, funcsObj) {
		canvas.onmouseenter = e => this.mouseOverCanvasFuncs = this.canvasMap.get(canvas)
		canvas.onmouseleave = e => this.mouseOverCanvasFuncs = null
		
		canvas.addEventListener("wheel", e => {
			e.preventDefault()
			
			if (this.mouseOverCanvasFuncs != null)
				this.mouseOverCanvasFuncs.scaleCallback(e.wheelDeltaY > 0 ? 1.12 : 0.892857)
		})
		
		super.addCanvas(canvas, funcsObj)
	}
	
	#mouseMoveHandler(e) {
		if (this.activeCanvasFuncs != null) {
			if (this.interactActive)
				this.activeCanvasFuncs.moveInteractCallback(e)
			
			else if (this.scaleActive) {
				let mul = (e.movementY / 300) + 1
				mul = mul < 0 ? mul : mul**-1
				
				this.activeCanvasFuncs.scaleCallback(mul)
			}
			
			else
				this.activeCanvasFuncs.moveCallback(e.movementX, e.movementY)
		}
	}
}

class TouchInputManager extends InputManager {
	constructor() {
		super()
		this.inputPrecision = 0.4
		this.touchPos = [0, 0]
		
		this.twoTouches = false
		this.touchesCenter = null
		this.touchesDist = null
		
		document.addEventListener("touchmove", this.#touchMoveHandler.bind(this), {passive: false})
		document.addEventListener("touchstart", e => {
			this.activeCanvasFuncs = this.canvasMap.get(e.target)
			
			if (this.activeCanvasFuncs != null) {
				this.touchPos = [e.touches[0].clientX, e.touches[0].clientY]
				
				if (e.touches.length > 1) {
					this.twoTouches = true
					this.#updateCenterAndDist(e.touches)
				}
				else {
					this.twoTouches = false
					
					let touchEvent = e.touches[0]
					touchEvent.x = touchEvent.clientX
					touchEvent.y = touchEvent.clientY
					this.activeCanvasFuncs.mouseDownCallback(touchEvent)
				}
			}
		})
		document.addEventListener("touchend", e => {
			if (this.activeCanvasFuncs != null)
				this.activeCanvasFuncs.mouseUpCallback(e)
		})
	}
	
	#touchMoveHandler(e) {
		if (this.activeCanvasFuncs != null) {
			if (this.twoTouches) {
				let prevCenter = this.touchesCenter
				let prevDist = this.touchesDist
				this.#updateCenterAndDist(e.touches)
				
				this.activeCanvasFuncs.moveCallback(this.touchesCenter[0] - prevCenter[0], this.touchesCenter[1] - prevCenter[1])
				this.activeCanvasFuncs.scaleCallback(this.touchesDist / prevDist)
			}
			else {
				let lastTouchPos = this.touchPos
				this.touchPos = [e.touches[0].clientX, e.touches[0].clientY]
				
				let touchEvent = e.touches[0]
				touchEvent.movementX = this.touchPos[0] - lastTouchPos[0]
				touchEvent.movementY = this.touchPos[1] - lastTouchPos[1]
				this.activeCanvasFuncs.moveInteractCallback(touchEvent)
			}
			
			if (e.cancelable)
				e.preventDefault()
		}
	}
	
	#updateCenterAndDist(touches) {
		this.touchesCenter = [(touches[1].clientX + touches[0].clientX) / 2, (touches[1].clientY + touches[0].clientY) / 2]
		this.touchesDist = ((touches[1].clientX - touches[0].clientX)**2 + (touches[1].clientY - touches[0].clientY)**2)**0.5
	}
}

export const setWindowSelect = isEnabled => document.children[0].style["-webkit-user-select"] = isEnabled ? "" : "none"
export const isTouchScreen = () => "ontouchstart" in window || navigator.maxTouchPoints > 0
export const INPUT_MANAGER = isTouchScreen() ? new TouchInputManager() : new MouseInputManager()

export class CanvasManager {
	constructor(canvas, drawFunc, resolutionMul = 2) {
		this.resolutionMul = resolutionMul
		
		this.canvas = canvas
		this.context = this.canvas.getContext("2d")
		this.draw = drawFunc
		
		this.curPos = [0, 0]
		this.curScale = 1
		
		INPUT_MANAGER.addCanvas(canvas, {
			moveCallback: this.#move.bind(this),
			scaleCallback: this.#scale.bind(this),
			mouseDownCallback: e => {},
			mouseUpCallback: e => {},
			moveInteractCallback: e => {}
		})
	}
	
	resetView() {
		this.curPos = [0, 0]
		this.curScale = 1
		
		this.#updateTransformMat()
	}
	
	redraw() {
		this.#clearCanvas()
		this.draw()
	}
	
	#clearCanvas() {
		this.context.save()
		this.context.setTransform(1, 0, 0, 1, 0, 0)
		this.context.clearRect(0, 0, this.canvas.width, this.canvas.height)
		this.context.restore()
	}
	
	#updateTransformMat() {
		this.context.setTransform(this.curScale, 0, 0, this.curScale,
			this.curPos[0] - this.canvas.width * (this.curScale - 1) / 2,
			this.curPos[1] - this.canvas.height * (this.curScale - 1) / 2)
	}
	
	#move(xMove, yMove) {
		this.curPos[0] += xMove * this.resolutionMul
		this.curPos[1] += yMove * this.resolutionMul
		
		this.#updateTransformMat()
		this.redraw()
	}
	
	#scale(mul) {
		this.curScale *= mul
		this.curPos[0] *= mul
		this.curPos[1] *= mul
		
		this.#updateTransformMat()
		this.redraw()
	}
	
	setScale(scale) {
		this.curScale = scale
		this.curPos = [0, 0]
		
		this.#updateTransformMat()
	}
	
	getLineScale() {
		return this.curScale / this.resolutionMul
	}
	
	containerPosToCanvas(x, y) {
		return [x * this.resolutionMul, y * this.resolutionMul]
	}
	
	setSize(width, height) {
		this.canvas.width = width * this.resolutionMul
		this.canvas.height = height * this.resolutionMul
		
		this.canvas.style.width = width + "px"
		this.canvas.style.height = height + "px"
	}
}