
.. highlight:: CDot
   :linenothreshold: 3

.. _basic-operators:

Basic Operators
===============

CDot supports most of the basic arithmetic, logical and assignment operators you are probably familiar with from other languages. Unlike many of those other languages though, every operator in CDot is actually just a normal function or method that can be called with operator syntax. You can also define your own operators with custom spellings, for more information on how to do that see :ref:`functions`.

Operators come in three forms:

    * **Infix** operators, which operate on two values and are written between the two
    * **Prefix** operators, which operate on one value and are written before it
    * **Postfix** operators, which operate on one value and are written after it

Arithmetic Operators
--------------------

All numeric types (i.e. integer and floating point types) support the familiar arithmetic infix operators ``+``, ``-``, ``*`` and ``/``. In contrast to their C equivalents, these operators check for overflow and exit the application with a fatal error when overflow (or division by zero) is encountered. You can opt in to the usual behavior of silently allowing overflow by using ``&``-prefixed variants of these operators::

    let add1 = 312 + 328 // OK
    let add2: Int8 = 127 + 1 // fatal error: unexpected integer overflow!
    let add3: Int8 = 127 &+ 1 // OK: yields -128

In addition to the default arithmetic operators, CDot also supports *remainder* and *exponentiation* operators, which are spelled ``%`` and ``**``, respectively. ::

    let rem = 125 % 12 // Yields 5
    let exp = 10 ** 5 // Yields 100,000

Prefix Arithmetic Operators
---------------------------

There are two additional unary arithmetic operators, ``+`` and ``-``. The prefix ``-`` operator can be used to negate a  numeric value, while the prefix ``+`` operator has no real purpose except for providing symmetry::

    let negOne      = -1
    let stillNegOne = +negOne
    let posOne      = -stillNegOne

Assignment Operators
--------------------

As you have seen in previous examples, variables can be assigned a new value using the *assignment* operator ``=``. To avoid accidental confusion between this operator and the equality comparison operator ``==``, assignment does not return a value in CDot::

    var v = 210
    v = 32

    if v = 32 {} // error: assignment does not yield a value

Cdot also supports *compound assignment* operators for all of the arithmetic operators, which combine an arithmetic computation with an assignment. These are spelled as ``{op}=`` and can be used like this::

    var v = 128
    v *= 3 // Multiply v by three and assign the result back to v
    v -= 10 // Subtract ten from v and assign the result back to v
    print(v) // Prints "374"

Comparison Operators
--------------------

Most types support the equality comparison operators ``==`` and ``!=``. Numeric types additionally support the comparison operators ``<``, ``>``, ``<=`` and ``>=``. All of these yield a boolean value and have the expected behavior. ::

    if 3 == 4 {
        print("well that's odd") // This will never run
    }

    var a = 3 > 4  // a is false
    var b = 3 < 4  // b is true
    var c = 3 <= 3 // c is true
    var d = 8 >= 9 // d is false

.. note::
    All of these operators are actually implemented using another operator ``<=>``, commonly called the *spaceship* operator. This operator is required to return a negative value if the left-hand operand is smaller than the right, zero if they are equal, and a positive value if the left operand is greater. Implementing this operator for your own types is actually enough to get all six other comparison operators.

There are two additional comparison operators, ``===`` and ``!==``, which are called the *identity comparison* operators. These can only be used on values of :ref:`class <classes>` type, and determine whether or not two values refer to the same *instance* of a class::

    let a = MyClass()
    let b = MyClass()
    let c = a

    print(a === c) // Prints "true"
    print(b === c) // Prints "false"

Logical Operators
-----------------

Logical operators operate on boolean values. There are three builtin logical operators:

    * Logical Not ``!``, a prefix operator
    * Logical And ``&&``, an infix operator
    * Logical Or ``||``, an infix operator

The logical not operator is used to *flip* the value of a ``Bool``, which means that it turns a ``true`` into a ``false`` and vice versa. ::

    if !condition {
        // do something if condition is false
    }

The logical *and* and *or* operators are special because they use *short-circuiting*, which means that the right-hand side expression will only be evaluated if it is necessary to determine the result of the expression. The ``&&`` operator will evaluate to ``true`` if both operands evaluate to ``true``, and will only evaluate the right-hand side expression if the left-hand side evaluated to ``true``. Similarly, the ``||`` operator will evaluate to ``true`` if either of the operands does, and will only evaluate the right-hand side if the left was not already ``true``. ::

    // `condition2` is only called if `condition1` returns true
    if condition1() && condition2() {
        // do something...
    }

    // `condition4` is only called if `condition3` returns false
    if condition3() || condition4() {
        // do something...
    }

Bitwise Operators
-----------------

Bitwise operators operate on the individual bits of an integer value. There are several of them in CDot:

    * Bitwise Not ``~``, a prefix operator
    * Bitwise Or ``|``, an infix operator
    * Bitwise And ``&``, an infix operator
    * Bitwise Xor ``^``, an infix operator

The bitwise *not* operator flips the value of each individual bit::

    var binaryValue = 0b0100_1010
    print((~binaryValue).toString(base: 2)) // Prints 0b1011_0101

The remaining bitwise operators compare the bits of two values and set the result bit according to their specific behaviour:

    * ``|`` sets the result bit if *either* of the operand bits were one
    * ``&`` sets the result bit if *both* of the operand bits were one
    * ``^`` sets the result bit if *exactly one* of the operand bits is one

Here's an example of how they work in action::

    var value1 = 0b1010
    var value2 = 0b1100

    print((value1 & value2).toString(base: 2)) // Prints "0b1000"
    print((value1 | value2).toString(base: 2)) // Prints "0b1110"
    print((value1 ^ value2).toString(base: 2)) // Prints "0b0110"

.. _optional-operators:

Optional Operators
------------------

CDot provides additional operators to make dealing with :ref:`optional values <optionals>` more convenient:

    * Force Unwrap ``!``, a postfix operator
    * Safe Unwrap ``??``, an infix operator

The *force unwrap* operator is used when you are certain that an optional contains a value. It will return the contained value or trap with a runtime failure if there is none. ::

    var opt1: Int? = 3
    print(opt1!) // Prints "3"

    var opt2: Int? = none
    print(opt2!) // fatal error: force unwrapping a `none` value!

Because this operator has the ability to trap, you should only use it when you are absolutely positive that an operation returns a value. In cases where you are not sure, you can use the *safe unwrap* operator instead. This operator will evaluate and return the right-hand side value if the left-hand side optional is ``none``::

    let defaultValue = 17
    var optionalValue: Int? = none
    var safeValue = optionalValue ?? defaultValue

    print(safeValue) // Prints "17"

This operator is also *short-circuiting*, which means that the default value is only evaluated if the optional value is ``none``.

.. note::
    The short-circuiting behavior of the ``??`` operator is made possible by a special :ref:`attribute <attributes>`, which will be explained in more detail later.

Range-Formation Operators
-------------------------

CDot also provides three convenience operators for forming *ranges*, one of which you have already seen in previous examples. These operators are a shorthand for forming range expressions, and are commonly used in ``for-in`` loops. The three range formation operators are:

    * Exclusive Range Operator ``..``, an infix operator
    * Inclusive Range Operator ``..=``, an infix operator
    * Half Open Range Operator ``..``, a postfix operator

The *exclusive* range operator forms a range from the left-hand side up to the right-hand side (exclusive), while the *inclusive* version forms a range that includes the upper bound. The half open range operator is special in that it does not provide an upper bound, instead continuing on forever. ::

    // Exclusive range, prints "0 1 2 3 4"
    for i in 0..5 {
        print(i)
    }

    // Inclusive range, prints "0 1 2 3 4 5"
    for i in 0..=5 {
        print(i)
    }

    // Half open range, prints "0 1 2 3 4 5 6 7 ..."
    for i in 0.. {
        print(i)
    }
