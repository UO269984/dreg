
class InputManager {
	constructor() {
		this.canvasMap = new Map()
		this.mouseOverCanvasFuncs = null
		this.activeCanvasFuncs = null
		
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
				
				this.interactActive = isInteractBt
				if (isInteractBt)
					this.activeCanvasFuncs.mouseDownCallback(e)
			}
		}
		document.onmouseup = e => {
			if (e.button == 0 || e.button == 1)
				this.activeCanvasFuncs = null
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
		this.canvasMap.set(canvas, funcsObj)
	}
	
	addCanvasAction(canvas, actionName, func) {
		this.canvasMap.get(canvas)[actionName] = func
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

export const INPUT_MANAGER = new InputManager()

export class CanvasManager {
	constructor(canvas, drawFunc) {
		this.canvas = canvas
		this.context = this.canvas.getContext("2d")
		this.draw = drawFunc
		
		this.curPos = [0, 0]
		this.curScale = 1
		
		INPUT_MANAGER.addCanvas(canvas, {
			moveCallback: this.#move.bind(this),
			scaleCallback: this.#scale.bind(this),
			mouseDownCallback: e => {},
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
		this.curPos[0] += xMove
		this.curPos[1] += yMove
		
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
}