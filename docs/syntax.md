# Grime Hello World
```
func main(argc: Int, argv: [String]) -> (exit: Int) {
	println("HelloWorld!")
	exit = 0
}
```

# Grime Syntax

## Types

### Core Types

#### Booleans

`Bool` - a boolean value

#### Integers

`Byte` - an 8bit integer  
`Short` - a 16bit integer  
`Int` - a 32bit integer  
`Long` - a 64bit integer  

`UByte` - an 8bit unsigned integer  
`UShort` - a 16bit unsigned integer  
`UInt` - a 32bit unsigned integer  
`ULong` - a 64bit unsigned integer

#### Reals

`Half` - a 16bit floating point real  
`Float` - a 32bit floating point real  
`Double` - a 64bit floating point real  

#### Characters

`Char` - an 8bit character encoded in UTF-8

#### Others

`[T]` - an array of data of type `T`  
`[T; n]` - a fixed size array of type `T` and size `n`  
`(T, U, V ... )` - a tuple of the types listed in the parenthesis  
`(x: T, y: U, z: V ... )` - a tuple with types and their references listed in the parenthesis  
`(T, U, V ... ) -> Z` - a function pointer  

#### Strings

`"This is a string!"` - defines a string  

## Functions

### Defining Functions

#### Return Void
```
func name(paramName: ParamType) {
	return
}
```

#### Return Other Types
```
func name(paramName: ParamType) -> ReturnType {

}
```

## Expressions

### Varibles and Values

`val x: Int = 0` Immutable value
`var y: Int = 0` Mutable value

### Selection
```
if (x < 10) {
	println("if")
} elif {
	println("elif")
} else {
	println("else")
}
```

```
if (x < 10) -> Int {
	return x + 1
} elif (x < 0) {
	return 0
} else {
	return x - 1
}
```

### Iteration
```
for (i in [0, 1, 2, 3]) {
	println(i)
}
```

```
val x: [Int] = for (i: Int in [0, 1, 2, 3]) -> Int {
	return i + 1
}
```

```
val x: Int = 10
while (x > 0) {
	x--;
}
```

```
val x = 10

val y: [Int] = while (x > 0) -> Int {
	x--;
	return x * 2;
}
```

## Tuples

### Named Tuples

`(x: Float, y: Int)`

#### Accessing Elements

```
val a: (x: Float, y: Int) = (1.0f, 10)

println(a.x)
println(a.y)
```

### Unnamed Tuples

`(Float, Int)`

#### Accessing Elements

```
val a: (Float, Int) = (1.0f, 10)

println(a.0)
println(a.1)
```


## Mutable Variables

```
var a: Int = 10
a = 11
```

This compiles because the value of `a` can change.

```
val a: Int = 10
a = 11
```

This will give a compile error because the value of `a` can't change.

```
var a: (Int, Float) = (10, 10.0f)
a.0 = 9
```

This does not compile because you can't change the elements of a tuple or array even if the variable is mutable. This also applies for object fields.

```
var a: (Int, Float) = (10, 10.0f)
a = (9, a.2)
```

This does compile.


## Skeletons & Objects

In grime skeletons define a new type. They provide a list of fields and methods for the user to call. Objects are implementations of skeletons. They provide a constructor and a destructor and implement all of the unimplemented methods and fields of the skeleton. You can not use Objects as a Type.

```

skeleton Animal {
	public val name: String


}

object Cat : Animal {
	override val food =
}
