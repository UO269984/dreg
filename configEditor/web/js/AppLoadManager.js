
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