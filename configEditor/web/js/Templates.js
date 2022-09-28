import {APP_LOAD_MANAGER} from "./AppLoadManager.js"

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

loadTemplate("templates/basicGraph.html", "basicGraph")
loadTemplate("templates/editableGraph.html", "editableGraph")
loadTemplate("templates/simGraph.html", "simGraph")