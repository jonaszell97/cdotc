
.. highlight:: CDot
   :linenothreshold: 3

The Basics
==========

Variables & Constants
---------------------

Variables and constants are the basic building blocks of every program; they allow you to assign a name to a value. The value of a constant, introduced by the ``let`` keyword, cannot change once it has been assigned. Variables on the other hand, which are introduced with the ``var`` keyword, can change their value at any time.

The following piece of code introduces a variable named ``myFavoriteColor`` and assigns it the value ``blue``. Since your preferences may change over time, we declare this as a variable with the ``var`` keyword, which allows us to reassign a different value later::

    var myFavoriteColor = "blue"
    // A few years later...
    myFavoriteColor = "green"

On the other hand you know that your birthday is never going to change, so you would create a constant to store it. If we try to reassign it later, the compiler would rightfully complain::

    let myBirthday = "02/29/1993"
    myBirthday = "01/01/2000" // error: trying to reassign constant

Variable names can include almost any valid unicode character, excluding punctuation and operator characters like ``+`` or ``&``. If you *really* need to use other characters in a variable name, you can escape it using *backticks*::

    let 🐅👑 = "Joe"
    let `the quick brown fox jumps over the lazy dog!!!` = 3

.. note::
    Generally speaking, you should always prefer to use a constant unless you know that the value needs to change after declaration.

Type Annotations
----------------

In the previous examples we didn't specify a type for the variables we declared. This is possible because CDot can *infer* the type of an expression based on the contents of that expression. In cases where the type can't be inferred or you need to specify a type different from the inferred type, you can provide a manual type annotation with a ``:`` colon following the variable name.  The two following declarations are semantically equivalent, because CDot infers the type of an integer literal to be ``Int`` if no other information is provided::

    let myInferredConstant = 0
    let myInferredConstant: Int = 0

If you need a different type, you can override this default type with a manual type annotation, like so::
    
    let myCustomTypedConstant: UInt8 = 0

Usually, CDot will be able to infer the type of almost any expression, so manual type annotations are rarely required.

Comments & Semicolons
---------------------

Similar to most C-like languages, CDot supports both single-line and block comments. Single line comments are introduced with two forward slashes and span until the end of the current line. Block comments begin with ``/*`` and end with ``*/`` and can span over multiple lines. Block comments can also be nested, so make sure the number of opening ``/*`` and closing ``*/`` match up::

    // This is a single line comment that ends here ->
    let myConstant = 12

    /*
        This is a multi-line comment. I can write whatever I want
        in here, even newlines.
        /*    This is a nested block comment.  */
    */
    let myOtherConstant = 42

Unlike C, you are not required to terminate statements with a semicolon in CDot. Sometimes inserting one is necessary for disambiguation though, especially when writing multiple statements on a single line::

    let noSemicolonNecessary = 3.0
    let optionalSemicolon = 21;
    let semicolonRequired = 79; print(semicolonRequired)

Integer Types
-------------

CDot provides its own versions of the familiar integer types from C. These include signed and unsigned integer types with bitwidths of 8, 16, 32, 64 and 128. Like all type names in CDot, integer types start with a capital letter. As an example, the signed integer type with a size of 8 bits is called ``Int8``, while the unsigned integer type with 32 bits is called ``UInt32``. CDot provides two additional integer types, ``Int`` and ``UInt``, which are equivalent to the signed and unsigned variants of the platforms word size, so on a 64-bit machine, ``Int`` is equivalent to ``Int64``. ::

    let v1 = 219
    let v2 = -2191
    print(v1 + v2) // Prints -1,972

The easiest way to create an integer is with an *integer literal*, like we have done in the previous examples. Integer literals can start with a minus sign to create a negative value, and they can includes underscores at arbitrary positions to enable better readability. Additionally, you can write binary, octal, and hexadecimal integer literals with the prefixes ``0b``, ``0``, and ``0x``, respectively. ::

    let largeInt = 1_000_000_000
    let binaryInt = 0b000101001010101
    let octalInt = 0216125
    let hexInt = 0x12ABC3EF

.. note::
    Usually, you should always use ``Int`` unless using a different type is really necessary. This makes interoperability between your code and other libraries easier. Even on 32-bit platforms, ``Int`` can store values between -2,147,483,648 and 2,147,483,647, which should be enough for most use cases.

Floating Point Types
--------------------

CDot provides two floating point types for dealing with real numbers, called ``Float`` and ``Double``. ``Float`` is equivalent to the ``float`` type in C, and usually has a precision of 32 bits, while ``Double`` has a precision of 64 bits on most platforms. The default type of a *floating point literal* is assumed to be ``Double``, if no other annotation is provided. ::

    let PI = 3.141592 // The type of PI is Double
    let E: Float = 2.71828 // The type of E is Float

Booleans
--------

A boolean is a type that can only take on two values, ``true`` and ``false``. Booleans are important because they are the key to many of the language's control flow statements. Booleans are introduced with a *boolean literal*::

    if true {
        // This will always be executed.
    }

    while false {
        // This will never be executed.
    }

.. note::
    Unlike other languages, integers cannot be used in place of a boolean in CDot. You need to manually write ``!= 0`` to use integers in a boolean condition.

.. note::
    For more information about ``if`` and ``while`` statements, refer to chapter :ref:`control-flow`.

Strings
-------

Strings are the default type for handling textual data in CDot. Strings are created using quoted *string literals*, and can contain any valid UTF-8 character::

    let myPrettyString = "The quick brown 🦊 jumps over the lazy 🐶"
    print(myPrettyString)

CDot also includes *string interpolation*, which allows you to embed any arbitrary string-convertible expression within a string literal. Interpolations are introduced with a dollar sign and braces::

    let name = "Peter"
    let currentYear = 2020
    let birthYear = 1993
    print("Hi, my name is ${name} and I am ${currentYear - birthYear} years old")
    // Prints "Hi, my name is Peter and I am 27 years old"

Collections
-----------

CDot provides special literals for common collection types like ``Array`` and ``Dictionary``, both of which can be introduced using angle brackets ``[]``::

    let vowels = ['a', 'e', 'i', 'o', 'u']
    print(vowels[3]) // Prints 'o'

    let cityPopulations = [
        "Berlin": 3_769_000,
        "New York City": 8_399_000,
        "Paris": 2_148_000,
    ]
    print(cityPopulations["Berlin"]!) // Prints 3769000

.. note::
    While the default type for array and dictionary literals are ``Array`` and ``Dictionary`` respectively, CDot allows you to enable literal initialization for custom collection types as well. See chapter :ref:`expressible-by` for more details. 

.. note::
    The ``!`` is necessary in the second print statement because accessing a value in a ``Dictionary`` like this returns an ``Optional``, which are explained in more detail in a :ref:`later chapter <optionals>`.

Optionals
---------

Optional types play an important role in CDot's type system. An ``Option`` type is a wrapper around another type, with the added caveat that there *may* or *may not* be an actual value stored in the optional. Optionals are introduced with a question mark following the type name::

    var optionalValue: Int? = 3

Any value is implicitly convertible to the corresponding optional type, which is why we can initialize an ``Int?`` with a literal of type ``Int``. While this creates an optional with a value, there is also a special keyword for the absence of a value::

    optionalValue = none

The ``none`` keyword tells the compiler that there is no value here. Because an optional my not actually have a value, most operations on optionals need to check whether or not there is a value first. For more information on how to do this, refer to :ref:`optionals`.

Tuples
------

In addition to named structure types, CDot also supports *tuples*, which are an easy way to group two or more related values. Tuples are spelled as a comma-separated list of values, and can be used whereever an expression is expected::

    var person = ("Joe", 54, true)
    print(person.0) // Prints "Joe"
    person.1 += 1 // Tuples can be modified

Values on a tuple can be accessed using a period followed by the zero-based index of the element you want to access.

Type Safety
-----------

Even though there are usually few explicit type annotations in a CDot program, it is a *type-safe* language, which means that it will check that the operations you perform on a value are valid for the type of that value. For example, if you try to subtract a string from an integer, you will get an error::

    let hmmm = 3 - "hello" // error: no implicit conversion from String to Int

Compile-time type checking removes a whole class of possible bugs from the language, because you can always be sure that the functions you call are called with the expected parameter types. At the same time, you lose some of the flexibility of a dynamic language like Python, because every variable has a fixed type::

    var onceAnInt = 219
    onceAnInt = "Hello, World!" // error: cannot assign String to variable of type Int

.. note::
    Coming from a language like Python or JavaScript, static typing may seem like an annoyance at first, but you will quickly realize that programs are much easier to reason about when everything has a fixed type. If you *really* need dynamic typing, CDot provides that in the form of :ref:`protocols`.

Printing Values
---------------

As you have seen in previous examples, the easiest way to output values to the console in CDot is using the global ``print(_:separator:terminator:)`` function (if you do not understand what the underscores and colons mean in the function signature, don't worry: this will be explained in the :ref:`functions` chapter). This function takes optional separator and terminator arguments, which allow you to specify what character to print between different values as well as what character to print after the final value. By default, the separator string is ``", "``, and the terminator is a newline. ::

    print("Hello", terminator: "") // Prints "Hello" without a newline at the end
    print(3, 4, separator: ":", terminator: ".") // Prints "3:4."
