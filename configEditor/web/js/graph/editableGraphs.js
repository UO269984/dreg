import {INPUT_MANAGER} from "./CanvasManager.js"
import {Graph} from "./Graph.js"

export class EditableGraph extends Graph {
	constructor(canvas, refsMapper, updateCallback, axisX, axisY, refs) {
		
		super(canvas, axisX, axisY)
		this.refsMapper = refsMapper
		this.updateCallback = updateCallback
		this.references = refs
		this.selectedRef = null
		this.selectedRefPos = -1
		
		INPUT_MANAGER.addCanvasAction(canvas, "mouseUpCallback", e => {
			if (this.selectedRef != null)
				this.updateCallback()
		})
		INPUT_MANAGER.addCanvasAction(canvas, "mouseDownCallback", this.mouseDown.bind(this))
		INPUT_MANAGER.addCanvasAction(canvas, "moveInteractCallback", this.mouseMove.bind(this))
		this.updateValues(this.refsMapper.getPoints())
	}
	
	mouseDown(e) {
		this.selectedRef = null
		this.selectedRefPos = -1
		
		let width_2 = this.canvas.width / 2
		let height_2 = this.canvas.height / 2
		
		let boundingRect = this.canvas.getBoundingClientRect()
		let canvasCoords = this.canvasManager.containerPosToCanvas(e.x - boundingRect.left, e.y - boundingRect.top)
		
		let mouseCoords = this.canvasToCoords(
			(canvasCoords[0] - this.canvasManager.curPos[0] - width_2) / this.canvasManager.curScale + width_2,
			(canvasCoords[1] - this.canvasManager.curPos[1] - height_2) / this.canvasManager.curScale + height_2)
		
		let targetSizeMul = this.canvasManager.getLineScale() * INPUT_MANAGER.inputPrecision
		let i = 0
		
		for (let ref of this.references) {
			if (Math.abs(mouseCoords[0] - ref[0]) < 7 / (this.config.mulAxisX * targetSizeMul) &&
				Math.abs(mouseCoords[1] - ref[1]) < 7 / (this.config.mulAxisY * targetSizeMul)) {
				
				this.selectedRef = ref
				this.selectedRefPos = i
				break
			}
			i++
		}
		this.canvasManager.redraw()
	}
	
	mouseMove(e) {
		if (this.selectedRef != null) {
			this.dragRef(e)
			this.updateRefValues()
		}
	}
	
	dragRef(e) {
		let canvasMov = this.canvasManager.containerPosToCanvas(e.movementX, -e.movementY)
		
		this.selectedRef[0] += canvasMov[0] / (this.config.mulAxisX * this.canvasManager.curScale)
		this.selectedRef[1] += canvasMov[1] / (this.config.mulAxisY * this.canvasManager.curScale)
	}
	
	addRef() {
		if (this.selectedRef != null) {
			let indexToAdd = this.references.indexOf(this.selectedRef) + 1
			if (indexToAdd == this.references.length)
				return
			
			let ref1 = this.references[indexToAdd - 1]
			let ref2 = this.references[indexToAdd]
			this.references.splice(indexToAdd, 0, [(ref1[0] + ref2[0]) / 2, (ref1[1] + ref2[1]) / 2])
			
			this.selectedRef = this.references[indexToAdd]
			this.updateRefValues()
		}
	}
	
	deleteRef() {
		if (this.selectedRef != null) {
			this.selectedRefPos = this.references.indexOf(this.selectedRef)
			
			if (this.selectedRefPos < 1 || this.selectedRefPos >= this.references.length - 1)
				return
			
			this.references.splice(this.selectedRefPos, 1)
			this.selectedRef = this.references[this.selectedRefPos - 1]
			this.updateRefValues()
			this.updateCallback()
		}
	}
	
	updateRefValues() {
		this.refsMapper.updateRefs(this.references)
		this.updateValues(this.refsMapper.getPoints())
	}
	
	drawGraph() {
		super.drawGraph()
		let handleSize = 2 / this.canvasManager.getLineScale()
		
		for (let curRef of this.references) {
			this.context.beginPath()
			this.context.arc(...this.coordsToCanvas(...curRef),
				curRef == this.selectedRef ? handleSize * 1.5 : handleSize, 0, 2 * Math.PI)
			
			this.context.fill()
		}
	}
}

export class BezierGraph extends EditableGraph {
	constructor(canvas, refsMapper, updateCallback, axisX, axisY, refs) {
		super(canvas, refsMapper, updateCallback, axisX, axisY, refs)
		this.refsAlignEnabled = true
	}
	
	toggleRefsAlign() {
		this.refsAlignEnabled = ! this.refsAlignEnabled
	}
	
	mouseMove(e) {
		if (this.selectedRef == null)
			return
		
		let prevPos = [this.selectedRef[0], this.selectedRef[1]]
		super.dragRef(e)
		let move = [this.selectedRef[0] - prevPos[0], this.selectedRef[1] - prevPos[1]]
		
		if (this.selectedRefPos % 3 == 0) { //Move references attached to the reference being dragged
			if (this.selectedRefPos > 0) {
				this.references[this.selectedRefPos - 1][0] += move[0]
				this.references[this.selectedRefPos - 1][1] += move[1]
			}
			if (this.selectedRefPos < this.references.length - 1) {
				this.references[this.selectedRefPos + 1][0] += move[0]
				this.references[this.selectedRefPos + 1][1] += move[1]
			}
		}
		
		else if (this.refsAlignEnabled) { //Move the opposite reference to the reference being dragged
			let toMoveRefDirection = (this.selectedRefPos - 1) % 3 == 0 ? -1 : 1
			let toMoveRefPos = this.selectedRefPos + toMoveRefDirection * 2
			
			if (toMoveRefPos >= 0 && toMoveRefPos < this.references.length) {
				let pivotRef = this.references[this.selectedRefPos + toMoveRefDirection]
				let toMoveRef = this.references[toMoveRefPos]
				
				let axisProp = this.config.mulAxisY / this.config.mulAxisX
				let dist = ((toMoveRef[1] - pivotRef[1])**2 + ((toMoveRef[0] - pivotRef[0]) / axisProp)**2)**0.5
				let m = (pivotRef[1] - this.selectedRef[1]) / ((pivotRef[0] - this.selectedRef[0]) / axisProp)
				
				let x = (dist**2 / (m**2 + 1))**0.5
				let y = x * m
				x *= axisProp
				
				if (this.selectedRef[0] > pivotRef[0]) {
					x = -x
					y = -y
				}
				toMoveRef[0] = pivotRef[0] + x
				toMoveRef[1] = pivotRef[1] + y
			}
		}
		this.updateRefValues()
	}
	
	addRef() {
		if (this.selectedRef != null) {
			if (this.selectedRefPos >= this.references.length - 2)
				return
			
			let indexToAdd = this.selectedRefPos + (3 - (this.selectedRefPos + 1) % 3)
			let ref1 = this.references[indexToAdd - 2]
			let ref2 = this.references[indexToAdd + 1]
			
			for (let w = 0.25; w < 1; w += 0.25)
				this.references.splice(indexToAdd++, 0, [ref1[0] * (1 - w) + ref2[0] * w, ref1[1] * (1 - w) + ref2[1] * w])
			
			this.selectedRefPos = indexToAdd - 2
			this.selectedRef = this.references[this.selectedRefPos]
			this.updateRefValues()
			this.updateCallback()
		}
	}
	
	deleteRef() {
		if (this.selectedRef != null) {
			if (this.selectedRefPos < 2 || this.selectedRefPos >= this.references.length - 2)
				return
			
			let firstToDeletePos = this.selectedRefPos - ((this.selectedRefPos - 2) % 3)
			this.references.splice(firstToDeletePos, 3)
			
			this.selectedRefPos = firstToDeletePos - 2
			this.selectedRef = this.references[this.selectedRefPos]
			this.updateRefValues()
			this.updateCallback()
		}
	}
	
	drawGraph() {
		super.drawGraph()
		this.context.lineWidth = 0.7 / this.canvasManager.getLineScale()
		
		for (let i = 0; i < this.references.length - 3; i += 3) {
			this.context.beginPath()
			this.context.moveTo(...this.coordsToCanvas(...this.references[i]))
			this.context.lineTo(...this.coordsToCanvas(...this.references[i + 1]))
			this.context.stroke()
			
			this.context.beginPath()
			this.context.moveTo(...this.coordsToCanvas(...this.references[i + 2]))
			this.context.lineTo(...this.coordsToCanvas(...this.references[i + 3]))
			this.context.stroke()
		}
	}
}