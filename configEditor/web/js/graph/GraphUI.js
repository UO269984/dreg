import {TEMPLATES} from "../AppLoadManager.js"
import {EditableGraph, BezierGraph} from "./editableGraphs.js"

export class BasicGraphUI {
	constructor() {
		this.scaleActive = false
		this.#initControls()
	}
	
	setGraph(graph) {
		this.graph = graph
	}
	
	#initControls() {
		this.container = document.createElement("div")
		this.container.innerHTML = this.getContainerHTML()
		this.container = this.container.children[0]
		
		this.canvas = this.container.getElementsByTagName("canvas")[0]
		this.canvas.width = 400
		this.canvas.height = 400
		
		this.container.getElementsByClassName("home-bt")[0].onclick = () => this.graph.resetView()
		this.container.getElementsByClassName("scale-bt")[0].onclick = this.toggleScale.bind(this)
	}
	
	getContainerHTML() {
		return TEMPLATES.basicGraph
	}
	
	toggleScale() {
		this.scaleActive = ! this.scaleActive
		
		let size = this.scaleActive ? 600 : 400
		this.canvas.width = size
		this.canvas.height = size
		this.graph.updateSize()
	}
}

export class EditableGraphUI extends BasicGraphUI {
	constructor(linearRefsToGraphFunc, bezierRefsToGraphFunc) {
		super()
		this.linearRefsToGraphFunc = linearRefsToGraphFunc
		this.bezierRefsToGraphFunc = bezierRefsToGraphFunc
		
		this.bezierMode = true
		this.#initControls()
		this.resetGraph()
	}
	
	#initControls() {
		this.modeBt = this.container.getElementsByClassName("graph-mode")[0]
		this.refsAlignedBt = this.container.getElementsByClassName("refs-align-bt")[0]
		this.modeBt.onclick = this.switchGraphMode.bind(this)
		this.refsAlignedBt.onclick = () => this.graph.toggleRefsAlign()
		
		this.container.getElementsByClassName("add-node-bt")[0].onclick = () => this.graph.addRef()
		this.container.getElementsByClassName("delete-node-bt")[0].onclick = () => this.graph.deleteRef()
		this.container.getElementsByClassName("reset-bt")[0].onclick = this.resetGraph.bind(this)
		
		let axisXMinInput = this.container.getElementsByClassName("axis-min-x")[0]
		let axisXMaxInput = this.container.getElementsByClassName("axis-max-x")[0]
		let axisYMinInput = this.container.getElementsByClassName("axis-min-y")[0]
		let axisYMaxInput = this.container.getElementsByClassName("axis-max-y")[0]
		
		let updateXAxis = () => this.graph.setXAxis({min: Number(axisXMinInput.value), max: Number(axisXMaxInput.value)})
		let updateYAxis = () => this.graph.setYAxis({min: Number(axisYMinInput.value), max: Number(axisYMaxInput.value)})
		axisXMinInput.addEventListener("input", updateXAxis)
		axisXMaxInput.addEventListener("input", updateXAxis)
		axisYMinInput.addEventListener("input", updateYAxis)
		axisYMaxInput.addEventListener("input", updateYAxis)
	}
	
	getContainerHTML() {
		return TEMPLATES.editableGraph
	}
	
	switchGraphMode() {
		this.bezierMode = ! this.bezierMode
		
		this.modeBt.innerText = this.bezierMode ? "Linear" : "Bezier"
		this.refsAlignedBt.style.display = this.bezierMode ? "" : "none"
		
		this.resetGraph()
	}
	
	resetGraph() {
		this.graph = new (this.bezierMode ? BezierGraph : EditableGraph)(this.canvas,
			this.bezierMode ? this.bezierRefsToGraphFunc : this.linearRefsToGraphFunc, {min: 0, max: 1}, {min: 0, max: 1})
	}
}