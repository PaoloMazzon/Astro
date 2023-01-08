import "lib/Renderer" for Renderer

class Main {
	static main() {
		System.print("Initializing renderer")
		Renderer.init("Game", 800, 600, Renderer.MSAA_32X, Renderer.SCREEN_MODE_TRIPLE_BUFFER, Renderer.FILTER_TYPE_NEAREST)

		System.print("Beginning loop")
		while (Renderer.update()) {
			System.print("Update")
		}

		System.print("Done")
	}
}