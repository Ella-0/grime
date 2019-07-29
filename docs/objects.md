# Objects

## Example
```
object Test {
  public val testVal : Int = 10
}
```

## Example with Inheritance

```
namespace xyz.grimelang.example {
  skeleton Printable {
    public val string: String
  }

  object Test {
    override public val string: String = "Test"
  }

  func println(x: Printable) -> Void {
    //...
  }

  func main(args: [String]) -> Int {
    val test: Test = Test()
    println(test)
  }
}
```

### How this would look in C

```c
struct g_skeleton_xyz_grimelang_example_Printable {
  void *data;
  void *child;
  void (construct *)(struct g_skeleton_xyz_grimelang_example_Printable);
  void (destruct *)(struct g_skeleton_xyz_grimelang_example_Printable);
  struct g_object_xyx_grimelang_core_String (g_getter_string*)(struct g_skeleton_xyz_grimelang_example_Printable);
};

struct g_object_xyz_grimelang_example_Test {
  struct g_skeleton_xyz_grimelang_example_Printable *parent_g_skeleton_xyz_grimelang_example_Printable;

};

struct g_object_xyz_grimelang_example_Test gen_g_object_xyz_grimelang_example_Test() {
  //Allocate memory and set function poiters for Test object and parent.
}

void g_println_g_oject_xyz_gimelang_code_Void_g_skeleton_xyz_grimelang_example_Printable(struct g_skeleton_xyz_grimelang_example_Printable x) {
  printf("%s\n", x.g_getter_string());
}

struct g_object_xyz_grimelang_core_Int g_main_g_object_xyz_grimelang_core_Int_g_object_xyz_grimelang_core_Array_g_object_xyz_grimelang_core_String(struct g_object_xyx_grimelang_core_Array_g_object_xyz_grimelang_core_String args) {
  const struct gen_g_object_xyz_grimelang_example_Test test = gen_g_object_xyz_grimelang_example_Test();
  test.construct(test);
  g_println_g_object_xyz_grimelang_core_Void_g_skeleton_xyz_grimelang_example_Printable(*test.parent_g_skeleton_xyz_grimelang_example_Printable);
  test.destruct(test);
}

int main(int argc, char ** argv) {

  //generate string array

  int status = g_main_g_object_xyz_grimelang_core_Int_g_object_xyz_grimelang_core_Array_g_object_xyz_grimelang_core_String(/*put generated string array here*/).data[0];

  return status;
}
```
