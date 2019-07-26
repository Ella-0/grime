# Name Mangling
## Types

### primatives

`Bool` - `o`

`Byte` - `b`
`Short` - `s`
`Int` - `i`
`Long` - `l`

`UByte` - `ub`
`UShort` - `us`
`UInt` - `ui`
`ULong` - `ul`

`Half` - `h`
`Float` - `f`
`Double` - `d`

`Char` - `c`

### Arrays

`[Int]` - `pi`
`[Int; 8]` - `pi8`

### Tuples

`(Byte, Short, Int, Long)` - `t4_b_s_i_l`

### Function pointer

`(Byte, Short, Int, Long) -> Bool` - `f4_o_b_s_i_l`

### Undescore

`_` - `__`

## Examples
### Functions

`func main() -> Int`
`func g_main_() -> Int`

`func add(a: Int, b: Int) -> Int`
`func g_add_i_i_i_(a: Int, b: Int) -> Int`

`func add(a: [Int], b: [Int]) -> [Int]`
`func g_add_pi_pi_pi_(a: [Int], b: [Int]) -> [Int]`

`func a(a: (x: String, y: String)) -> [Int]`
`func g_a_pi_t2_pc_pc_`

`func sort([Int], fun: (Int) -> Byte) -> [Int]`
`func g_sort_pi_pi_f1_b_i_([Int], fun: (Int) -> Byte) -> [Int]`
