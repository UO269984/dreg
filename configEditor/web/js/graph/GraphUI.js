import {TEMPLATES} from "../Templates.js"
import {Graph} from "./Graph.js"
import {EditableGraph, BezierGraph} from "./editableGraphs.js"

class GraphUI {
	constructor() {
		this.scaleActive = true
		this.#initControls()
		
		this.canvas = this.container.getElementsByTagName("canvas")[0]
	}
	
	#initControls() {
		this.container = document.createElement("div")
		this.container.innerHTML = this.getContainerHTML()
		this.container = this.container.children[0]
		
		this.container.getElementsByClassName("home-bt")[0].onclick = () => this.graph.resetView()
		this.container.getElementsByClassName("scale-bt")[0].onclick = this.toggleScale.bind(this)
	}
	
	getContainerHTML() {
		return TEMPLATES.basicGraph
	}
	
	toggleScale() {
		this.scaleActive = ! this.scaleActive
		
		let size = this.scaleActive ? 600 : 400
		this.graph.setSize(size, size)
	}
}

export class BasicGraphUI extends GraphUI {
	constructor() {
		super()
		this.graph = new Graph(this.canvas, {min: 0, max: 1}, {min: 0, max: 1})
		this.toggleScale()
	}
}

export class EditableGraphUI extends GraphUI {
	static defaultRefsLinear = [[0, 0], [1, 1]]
	static defaultRefsBezier = [[0, 0], [0.2, 0.2], [0.2, 0.4], [0.4, 0.6], [0.6, 0.8], [0.8, 0.8], [1, 1]]
	
	constructor(graphApi, updateGraphCallback) {
		super()
		this.graphApi = graphApi
		this.updateGraphCallback = updateGraphCallback
		this.refsMapper = {getPoints: () => this.graphApi.getPoints()}
			
		this.#initControls()
		this.updateInitData()
		this.toggleScale()
	}
	
	#initControls() {
		let modeBt = this.container.getElementsByClassName("graph-mode")[0]
		this.modeBtImg = modeBt.querySelector("img")
		this.refsAlignedBt = this.container.getElementsByClassName("refs-align-bt")[0]
		modeBt.onclick = this.switchGraphMode.bind(this)
		this.refsAlignedBt.onclick = () => {
			this.graph.toggleRefsAlign()
			this.#updateAlignedRefsBt()
		}
		
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
	
	#updateAlignedRefsBt() {
		this.refsAlignedBt.style.background = ! this.bezierMode || this.graph.refsAlignEnabled ? "" : "#50d94f"
	}
	
	updateInitData() {
		this.initData = this.graphApi.getInitData()
		this.bezierMode = this.initData.isBezier
		this.#updateBezierMode()
		
		this.#createGraph(this.initData.refs)
	}
	
	switchGraphMode() {
		this.bezierMode = ! this.bezierMode
		this.#updateBezierMode()
		
		this.resetGraph()
		this.#updateAlignedRefsBt()
	}
	
	resetGraph() {
		let refs = this.bezierMode == this.initData.isBezier ? this.initData.refs :
			this.bezierMode ? EditableGraphUI.defaultRefsBezier : EditableGraphUI.defaultRefsLinear
		
		this.refsMapper.updateRefs(refs)
		this.#createGraph(refs)
		this.updateGraphCallback()
	}
	
	#updateBezierMode() {
		let graphType = this.bezierMode ? "linear" : "bezier"
		this.modeBtImg.src = `icons/${graphType}.png`
		this.modeBtImg.alt = `${graphType} graph icon`
		this.refsAlignedBt.style.display = this.bezierMode ? "" : "none"
		
		this.refsMapper.updateRefs = this.bezierMode ?
			(refs => this.graphApi.loadBezierGraph(refs, 0)) :
			(refs => this.graphApi.loadLinearGraph(refs))
	}
	
	#cloneRefs(refs) {
		return refs.map(point => point.slice())
	}
	
	#createGraph(refs) {
		this.graph = new (this.bezierMode ? BezierGraph : EditableGraph)(this.canvas,
			this.refsMapper, this.updateGraphCallback, {min: 0, max: 1}, {min: 0, max: 1}, this.#cloneRefs(refs))
	}
}