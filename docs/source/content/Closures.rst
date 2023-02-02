
Closures
========

CDot supports creating and calling anonymous functions, which are often referred to as *lambda expressions* or *closures*. The name *closure* is based on the fact that these functions *close over* their environment, capturing referenced variables.

Explicit Closure Syntax
-----------------------

There are two syntaxes for creating closures in CDot. The first is based on C#'s closure syntax - a parameter list followed by a double arrow ``=>`` and the closure body. If there is only one parameter, the parentheses around the parameter list can be left out. Additionally, parameter types can be elided if they can be inferred from context, the same goes for the return type. If the closure body is a single expression, the braces around the body can be elided and the single expression is implicitly returned from the closure. The following closure expressions are thus equivalent::

    // Complete closure definition
    let addOne = (_ param: Int) -> Int => {
        return param + 1
    }

    // Elide parameter types, return type and braces
    let addOne = param => param + 1

In both cases, the type of ``addOne`` will be a *function type* with the signature ``(Int) -> Int``. Values of function type can be called just like normal functions, and the same rules for parameter labels apply. You could use addOne like this::

    let one = 1
    let two = addOne(one)
    let three = addOne(addOne(one))

Trailing Closure Syntax
-----------------------

Since closures are commonly passed as function arguments, CDot provides a second, convenient syntax for defining closures, called the *trailing closure* syntax. Trailing closures can appear after a function call, and they look like a normal block of code surrounded by braces. Since there is no explicit parameter list, parameters are referred to by a dollar sign followed by the zero-based parameter index within the closure body (e.g. ``$0``) for the first argument. The amount of arguments the closure takes is inferred based on the usage of the parameters within the body, while the parameter and return types are inferred from context. ::

    // Array of strings to be parsed as numbers
    let numberStrings = ["122", "-3812", "9919"]

    // Map each string to a number
    let numbers = numberStrings.map {
        return Int($0)
    }

    // Equivalent to...
    let numbers = numberStrings.map((s: String) -> Int => {
        return Int(s)
    })

Capturing Values
----------------

Closures can *capture* values from their environment, which means that every variable that is visible in the context within which the closure is created can be referenced and modified within the closure body. The compiler takes care of extending the lifetime of the captured values to match the lifetime of the closure. ::

    var s = "Hello, World!"
    let modifier = {
        s = "World, Hello?"
    }

    modifier()
    print(s) // Prints "World, Hello?"
