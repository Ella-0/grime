; ModuleID = 'xyz.grimelang.core'
source_filename = "types.ll"

%struct.g_object_xyz_grimelang_core_Bool = type {
  i1
}

%struct.g_object_xyz_grimelang_core_Byte = type {
  i8
}

%struct.g_object_xyz_grimelang_core_Short = type {
  i16
}

%struct.g_object_xyz_grimelang_core_Int = type {
  i32
}

%struct.g_object_xyz_grimelang_core_Long = type {
  i64
}

%struct.g_object_xyz_grimelang_core_Char = type {
  i8
}

%struct.g_skeleton_xyz_grimelang_core_List_g_object_xyz_grimelang_core_Char = type {
  void*, ; 0 child
  %struct.g_object_xyz_grimelang_core_Int(%g_skeleton_xyz_grimelang_core_List_g_object_xyz_grimelang_core_Char), ; 0 get size
  %struct.g_object_xyz_grimelang_core_Char(%g_skeleton_xyz_grimelang_core_List_g_object_xyz_grimelang_core_Char, %struct.g_object_xyz_grimelang_core_Int) ; 1 get data
}

%struct.g_object_xyz_grimelang_core_Array_g_object_xyz_grimelang_core_Char = type {
  %struct.g_skeleton_xyz_grimelang_core_List_g_object_xyz_grimelang_core_Char*, ; 0 super List<Char>
  %struct.g_object_xyz_grimelang_core_Int, ; 1 size Int
  %struct.g_object_xyz_grimelang_core_Char* ; 2 data
}

%struct.g_object_xyz_grimelang_core_String = type {

}
