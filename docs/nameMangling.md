# Name Mangling

This document describes how an implementation of the gime language should perform name mangling.

## Types

### primatives

`Bool` - `g_object_xyz_grimelang_core_Bool`

`Byte` - `g_object_xyz_grimelang_core_Byte`
`Short` - `g_object_xyz_grimelang_core_Short`
`Int` - `g_object_xyz_grimelang_core_Int`
`Long` - `g_object_xyz_grimelang_core_Long`

`UByte` - `g_object_xyz_grimelang_core_UByte`
`UShort` - `g_object_xyz_grimelang_core_UShort`
`UInt` - `g_object_xyz_grimelang_core_UInt`
`ULong` - `g_object_xyz_grimelang_core_ULong`

`Half` - `g_object_xyz_grimelang_core_Half`
`Float` - `g_object_xyz_grimelang_core_Float`
`Double` - `g_object_xyz_grimelang_core_Double`

`Char` - `g_object_xyz_grimelang_core_Char`

### Lists

`[Int]` - `g_skeleton_xyz_grimelang_core_List_g_object_xyz_grimelang_core_Int`
`[Char]` - `g_skeleton_xyz_grimelang_core_List_g_object_xyz_grimelang_core_Char`

### Tuples

`(Byte, Short, Int, Long)` - `t4_g_object_xyz_grimelang_core_Byte_g_object_xyz_grimelang_core_Short_g_object_xyz_grimelang_core_Int_g_object_xyz_grimelang_core_Long`

### Function pointer

`(T, U, V, W) -> X` - `f4_X_T_U_V_W`

`(Byte, Short, Int, Long) -> Bool` - `f4_g_skeleton_xyz_grimelang_core_Byte`

### Objects & Skeletons

`com.example.animals.Animal` - `g_skeleton_xyz_com_example_animals_Animal`
`com.example.animals.Cat` - `g_object_com_example_animals_Cat`

### Undescore

`_` - `__`

### functions

`func main(args: [String]) -> Int` - `g_main_g_object_xyz_grimelang_core_Int_g_skeleton_xyz_grimelang_core_List_xyz`
