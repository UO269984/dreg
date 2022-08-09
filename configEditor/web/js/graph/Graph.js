import {CanvasManager} from "./CanvasManager.js"
import UTIL from "./Util.js"

export class Graph {
	constructor(canvas, axisX, axisY) {
		this.values = [[], []]
		this.config = {}
		
		this.canvas = canvas
		this.canvasManager = new CanvasManager(this.canvas, this.drawGraph.bind(this))
		this.context = this.canvasManager.context
		
		this.#computeAxisStep(axisX)
		this.#computeAxisStep(axisY)
		this.config.axisX = axisX
		this.config.axisY = axisY
		
		this.#updateGraphConfig()
		this.canvasManager.setScale(0.9)
	}
	
	resetView() {
		this.canvasManager.resetView()
		this.canvasManager.setScale(0.9)
		this.canvasManager.redraw()
	}
	
	#updateGraphConfig() {
		this.config.mulAxisX = this.canvas.width / (this.config.axisX.max - this.config.axisX.min)
		this.config.mulAxisY = this.canvas.height / (this.config.axisY.max - this.config.axisY.min)
	}
	
	#updateAxis(axis) {
		this.#computeAxisStep(axis)
		
		this.#updateGraphConfig()
		this.canvasManager.redraw()
	}
	
	setXAxis(axisX) {
		if (axisX.min < axisX.max) {
			this.config.axisX = axisX
			this.#updateAxis(axisX)
		}
	}
	
	setYAxis(axisY) {
		if (axisY.min < axisY.max) {
			this.config.axisY = axisY
			this.#updateAxis(axisY)
		}
	}
	
	setAxisAutoFor(values, forceUpdate = false) {
		let axisXUpdated = this.#updateAxisAuto(this.config.axisX, values[0], forceUpdate, 0)
		let axisYUpdated = this.#updateAxisAuto(this.config.axisY, values[1], forceUpdate)
		
		if (axisXUpdated || axisYUpdated)
			this.#updateGraphConfig()
	}
	
	#updateAxisAuto(axis, values, forceUpdate = false, marginPercent = 0.1) {
		let min = UTIL.min(values)
		let max = UTIL.max(values)
		
		if (forceUpdate || min < axis.min || max > axis.max || (axis.max - axis.min) / (max - min) > 3.5) {
			if (min == max)
				max++
			
			let margin = (max - min) * marginPercent
			min -= margin
			max += margin
			
			axis.min = min
			axis.max = max
			this.#computeAxisStep(axis)
			return true
		}
		else
			return false
	}
	
	#computeAxisStep(axis) {
		let interval = axis.max - axis.min
		let minError = interval
		axis.step = interval
		
		for (let intervCount = 13; intervCount < 24; intervCount++) {
			let step = interval / intervCount
			let stepScale = 10**Math.floor(Math.log10(step))
			let roundedStep = Math.round(step / stepScale) * stepScale
			let error = Math.abs(step - roundedStep)
			
			if (error <= minError) {
				minError = error
				axis.step = roundedStep
			}
		}
		axis.textStep = axis.step * 2
	}
	
	setSize(width, height) {
		this.canvasManager.setSize(width, height)
		this.#updateGraphConfig()
		this.resetView()
	}
	
	updateValues(values) {
		this.values = values
		this.canvasManager.redraw()
	}
	
	drawGraph() {
		//Draw axis
		this.context.lineWidth = 1.5 / this.canvasManager.getLineScale()
		this.context.strokeStyle = "black"
		
		let originPos = this.coordsToCanvas(0, 0)
		
		this.context.beginPath()
		this.context.moveTo(originPos[0], 0)
		this.context.lineTo(originPos[0], this.canvas.height)
		this.context.stroke()
		
		this.context.beginPath()
		this.context.moveTo(0, originPos[1])
		this.context.lineTo(this.canvas.width, originPos[1])
		this.context.stroke()
		
		//Draw graph values
		this.context.beginPath()
		this.context.moveTo(...this.coordsToCanvas(this.values[0][0], this.values[1][0]))
		
		let valuesCount = this.values[0].length
		for (let i = 1; i < valuesCount; i++)
			this.context.lineTo(...this.coordsToCanvas(this.values[0][i], this.values[1][i]))
		
		this.context.stroke()
		
		//Draw grid
		this.context.lineWidth = 0.4 / this.canvasManager.getLineScale()
		this.context.strokeStyle = "#666666"
		this.context.font = "20px Arial"
		
		this.#drawGrid(this.config.axisX, val => {
			let xPos = this.coordsToCanvas(val, 0)[0]
			this.context.moveTo(xPos, 0)
			this.context.lineTo(xPos, this.canvas.height)
			
		}, val => this.coordsToCanvas(val, 0))
		
		this.#drawGrid(this.config.axisY, val => {
			let yPos = this.coordsToCanvas(0, val)[1]
			this.context.moveTo(0, yPos)
			this.context.lineTo(this.canvas.width, yPos)
			
		}, val => this.coordsToCanvas(0, val))
	}
	
	#drawGrid(axis, drawAxisLine, indexToCoordsCallback) {
		for (let i = axis.min + axis.step; i < axis.max; i += axis.step) {
			this.context.beginPath()
			drawAxisLine(i)
			this.context.stroke()
		}
		
		for (let i = axis.min + axis.textStep; i < axis.max; i += axis.textStep) {
			i = Math.round(i * 10000) / 10000
			let coords = indexToCoordsCallback(i)
			this.context.fillText(i, coords[0] + 5, coords[1] - 5)
		}
	}
	
	coordsToCanvas(x, y) {
		return [(x - this.config.axisX.min) * this.config.mulAxisX,
			this.canvas.height - ((y - this.config.axisY.min) * this.config.mulAxisY)]
	}
	
	canvasToCoords(x, y) {
		return [this.config.axisX.min + (x / this.config.mulAxisX),
			((this.canvas.height - y) / this.config.mulAxisY) + this.config.axisY.min]
	}
}