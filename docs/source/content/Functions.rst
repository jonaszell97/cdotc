
.. highlight:: CDot
   :linenothreshold: 3

.. _functions:

Functions
=========

*Functions* are specialized pieces of code that can be executed from different parts of your program. They allow you to avoid repetition and bundle common logic into reusable components. In CDot, functions are introduced with the ``def`` keyword followed by the function name. After that, the functions *parameter list* follows in parentheses. Functions can also declare an optional return type by writing a type name after a *single arrow* ``->``. ::

    def myFunction(_ param1: Int, _ param2: Float) -> String
    //  ^^^^^^^^^^   ^^^^^^         ^^^^^^            ^^^^^^
    //  Name         Parameter      Parameter         Return Type

.. note::
    If you do not explicitly write a return type, the functions return value is assumed to be of type ``Void``, which is just an alias for the empty tuple type ``()`` in CDot.

A function can take zero or more parameters, but the parentheses are always required, even for parameterless functions. After the function declaration, its *body* follows surrounded by braces. This is the actual code that is executed whenever your function is called. ::

    /// A function that takes two ``Int`` s and returns their sum.
    def addTwoInts(_ lhs: Int, _ rhs: Int) -> Int {
        return lhs + rhs
    }

.. note::
    Every function that has a return type needs to include a ``return`` statement at the end of every possible path through the function. For ``Void``-returning functions, a ``return`` statement is implicitly inserted for you at the end of the function body if you do not write one.

You can ``return`` from any point in a function, but every path through the function must end in a ``return`` statement (with the aforementioned exception of ``Void``-returning functions). For example, the following function definition is invalid::

    def badFunction() -> Int {
        if someCondition {
            return 12
        }

        // error: badFunction does not return if `someCondition` is false!
    }

Argument Labels
---------------

You may have wondered about the underscores in front of the parameter name in the first example - these are necessary because functions parameters are *labeled* by default. A parameter *label* has to be specified at the call site, which makes function calls easier to read and can help disambiguation. There are three different ways to label a function parameter:

    * Equal label and parameter name: If you do not specify a label, the parameter will implicitly be labeled with the parameter name.
    * Different label and parameter name: If you put a label before the parameter name, the label will be what callers of the function put in their call, while the parameter name is what the parameter can be referred to in the function body.
    * Unlabeled parameters: An underscore in place of a label will make the parameter *unlabeled*, which means that callers do not have to specify a label.

Labels should be used when the role of a parameter is not obvious from the context of the function call. By convention, labeled function calls should read like a sentence - as an example, you might define a function that divides an integer like this::

    def divide(_ value: Int, by rhs: Int) -> Int {
        return value / rhs
    }

    // Called like this:
    divide(12, by: 39) // Read as "divide 12 by 39"

Default Parameter Values
------------------------

Function parameters can declare an optional *default value*, which will allow callers to leave out that parameter in the call. An example for this is the global ``print`` function you are already familiar with. Its declaration looks something like this::

    def print(_ value: String,
              separator: String = ", ",
              terminator: String = "\\n") {
        // ...
    }

    // Can be called without the optional parameters...
    print("Hello, World")

    // ...or with one of them
    print("Hello, World", terminator: "")

    // ...or with both
    print("Hello, World", separator: " <> ", terminator: "")

.. note::
    Like other languages, CDot requires every parameter after one with a default value to also have a default value.

Ownership Semantics
-------------------

Function parameters can also declare an *ownership type*, which can be one of ``borrow``, ``ref``, ``mut ref``, or ``owned``. The exact semantics of these are explained in chapter :ref:`ownership`. By default, a function parameter will use the ``borrow`` ownership convention.

Function Overloading
--------------------

In CDot, you can define multiple functions with the same name, as long as their signatures are different in some way. The correct overload will then be chosen at the call site based on parameter types, labels, or even the return type. ::

    def print(_ i: Int)                      // #1
    def print(_ s: String)                   // #2
    def print(_ s: String) -> String         // #3
    def print(_ s: String, to: OutputStream) // #4

    print(3) // #1 chosen based on parameter type
    print("hello") // ambiguous: could be #2 or #3
    var s: String = print("hello") // #3 chosen based on return type
    print("hello", to: io.stderr) // #4 chosen based on parameter labels

It is an error if a function call cannot be uniquely resolved to a specific overload of a function.

.. note::
    Overload resolution also takes into account other factors such as *conversion penalties* or *genericness*. This means that a non-generic function will be preferred over a :ref:`generic <generics>` one, and that the overload that requires the minimum amount of conversions will be chosen.
