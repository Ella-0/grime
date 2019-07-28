# Grime Coding Standards

## Names

### Type Names
Type names start with a upper case letter and use cammel case. They can be defined by the following regular expression `[A-Z][A-Za-z]*`

### Variable Names

#### Global Variables
Global variable names should be allcaps with Undescores between words. Global variable are the only place in grime where Undescores should be used. They can be defined by the following regular expression.`[A-Z](_[A-Z]+)*`.

#### Local Variables
Local variables start with a lower case letter and use cammel case. They can be defined by the following regular expression `[a-z][A-Za-z]*`.

### Function Names
Functions are named in the same way as local variables. They are defined by the following regular expression `[A-Z](_[A-Z]+)*`.

# An Example Source File
```
func main(args: [String]) -> Int {

}
```
