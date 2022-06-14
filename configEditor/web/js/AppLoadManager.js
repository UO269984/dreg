
class AppLoadManager {
	constructor() {
		this.remainingTasks = 0
		this.onload = () => {}
	}
	
	setOnLoad(onLoadFunc) {
		this.onload = onLoadFunc
		
		if (this.isReady())
			this.onload()
	}
	
	isReady() {
		return this.remainingTasks == 0
	}
	
	addLoadTask() {
		this.remainingTasks++
		return this.#loaded.bind(this)
	}
	
	#loaded() {
		this.remainingTasks--
		
		if (this.isReady())
			this.onload()
	}
}

export const APP_LOAD_MANAGER = new AppLoadManager()

function loadTemplate(url, name) {
	let loadedCallback = APP_LOAD_MANAGER.addLoadTask()
	
	fetch(url).then(resp => {
		if (resp.status == 200) {
			resp.text().then(data => {
				TEMPLATES[name] = data
				loadedCallback()
			})
		}
		else
			console.error(`Failed to load template from: ${url}`)
	})
}

export const TEMPLATES = {}

loadTemplate("basicGraph.html", "basicGraph")
loadTemplate("editableGraph.html", "editableGraph")
loadTemplate("simGraph.html", "simGraph")