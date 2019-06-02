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

### Defining new Types

You can define types using type definitions. these are declared using the following syntax.

```
type Vector3<T> : (x: T, y: T, z: T)
```
### Adding functions to new types
```
func Vector3<Float>.add(right: Vector3f<Float>) -> Vector3<Float> {
	return Vector3<Float>(this.x + right.x, this.y + right.y, this.z + right.z)
}
```
