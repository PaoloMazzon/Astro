foreign class Vec3 {
	construct new(x, y, z) {}

	foreign add()
	foreign dot(vec)
}

class Math {
	foreign static add(a, b)

    static print() {
		var vec = Vec3.new(5, 5, 5)
		var vec2 = Vec3.new(10, 10, 10)
    	System.print(add(5, 5))
    	System.print(vec.dot(vec2))

    }
}

var newvec = Vec3.new(5, 5, 5)
System.print("Script")