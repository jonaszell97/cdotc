
.. highlight:: CDot
   :linenothreshold: 3

.. _control-flow:

Control Flow
============

CDot includes most of the familiar control-flow statements you know from other languages, along with some slightly modified ones. This chapter provides an overview of what they are and how to use them.

If-Statements
-------------

``if`` statements are the simplest way to change control flow in a program. An ``if`` statement takes a condition, and the code surrounded by the ``if`` is only executed when that condition evaluates to ``true``. Optionally, you can provide an ``else`` clause to execute if the condition is false, which can itself be another ``if`` statement. ::

    if calculateComplexCondition() {
        print("First condition was true 😃!")
    }
    else if calculateEvenMoreComplexCondition() {
        print("Other condition was true 🙂!")
    }
    else {
        print("No condition was true 😢...")
    }

.. note::
    The condition value does not have to be of type ``Bool``, any value that conforms to the ``TruthValue`` :ref:`protocol <protocols>` will do.

Instead of evaluating a boolean condition, an ``if`` statement can also bind a value, in which case the conditional code is only executed if that value is *truthy*. This is referred to as a *binding* statement, because the condition being evaluated is bound to a name that can be referenced in the ``if`` block. As an example, any :ref:`Optional <optionals>` value is considered *truthy* if it actually stores a value, in which case the stored value will be bound by the ``if`` statement. ::

    let mayHaveAValue: String? = "Good day to you"
    if let value = mayHaveAValue {
        print(value)
    }

As you can see, binding if statements are introduced using ``if let`` followed by the name to give the bound value. The previous example can be read as "if 'mayHaveAValue' actually has a value, unwrap it and bind it to a variable named 'value'".

.. note::
    Similar to a normal ``if``, ``if let`` statements are also not restricted to using Optional values. Any value that conforms to the ``Unwrappable`` :ref:`protocol <protocols>` can be used in a binding statement.

There is a third spelling of an ``if`` statement that allows you to *pattern match* a value. This is explained in a :ref:`later chapter <pattern-matching>`.

While-Statements
----------------

A ``while`` statement is similar to an ``if`` statement in that it allows you to specify a condition in the same way. The behaviours differs though, because the code in a ``while`` statement will be executed as long as the condition remains truthy, which can be zero or infinitely many times. Because of this, a ``while`` statement can not specify an ``else`` clause. ::

    var n = Int.random()
    while n > 1 { // May or may not ever terminate...
        if n % 2 == 0 {
            n = n / 2
        }
        else {
            n = 3 * n + 1
        }
    }

Loop-Statements
---------------

``loop`` statements are very similar to ``while`` statements, only that the condition is evaluated *after* the execution of the body, which means that the code is guaranteed to be executed at least once. You can also completely leave out the condition to create an infinite loop. ::

    var i = 0

    // This runs at least once
    loop {
        print(i)
        i += 1
    } while Int.random() != 100

    // This loop is semantically the same as the previous one
    loop {
        print(i)
        i += 1

        if Int.random() == 100 {
            break
        }
    }

For-Statements
--------------

CDot supports traditional C-Style for loops, which consist of semicolon-separated *initialization*, *termination* and *increment* parts. The *initialization* statement is executed once at the start of the loop, while the *increment* is executed after every iteration. The loop terminates once the *termination* evaluates to ``false`` for the first time. ::

    for var i = 0; i < 5; ++i {
        print(i, terminator: " ") // Prints "1 2 3 4 5 "
    }

.. note::
    There is almost no good reason to use this style of for-loops in CDot, unless you want very fine-grained control over your program's control-flow. In most cases, the ``for-in`` variant explained in the next section should be preferred.


For-In-Statements
-----------------

The ``for-in`` statement is a specialization of the usual for loop, which allows you to iterate over any arbitrary sequence of elements. To replicate the behaviour of the above for loop, you can simply iterate over a ``Range``::

    for i in 0..5 {
        print(i, terminator: " ")
    }

This can be read as *for every value in the sequence, assign that value the name ``i`` and execute the loop body*. This works for any type that implements the ``Iterable`` protocol, which includes any standard library collection::

    let favoriteIceCreamFlavors = ["Strawberry", "Vanilla", "Mint"]
    for flavor in favoriteIceCreamFlavors {
        if flavor == "Mint" {
            print("ewwww!")
        }
    }

Match-Statements
----------------

``match`` statements are CDot's variant of the familiar ``switch`` from other languages, but much more powerful. You can match the given value against any arbitrary pattern as explained in :ref:`pattern-matching`. In the simplest case, a ``match`` statement can look familiar to any ol' ``switch``::

    match readValue() {
    case 0:
        print("read a zero!")
    case 1:
        print("read a one!")
    default:
        print("read an unknown value!")
    }

This will execute the block after the first ``case`` if ``readValue()`` evaluates to zero, the second block if it evaluates to one, and the ``default`` block otherwise. Unlike C, match statements in CDot need to be *exhaustive*, which means that you either need to cover every possible value or you need to provide a ``default`` block.

.. note::
    There is no explicit fallthrough in a match statement, which means that a ``break`` is optional in a ``case`` block. If you need fallthrough behavior, you can explicitly ask for it with the ``continue`` keyword.

You can also provide custom matching behavior for any type. ``Range`` s for example, provide matching for the type of the range bounds::

    match 121 {
    case 0..10:
        print("very small")
    case 10..500:
        print("large-ish")
    default:
        print("large")
    }

    // Prints "large-ish"

More complex patterns are explained in :ref:`pattern-matching`.

Break & Continue
----------------

Loops introduced with ``while``, ``loop`` or ``for`` support ``break`` and ``continue`` statements within the loop body. The ``break`` statement will immediately end the execution of the loop and prevent any future iterations, regardless of whether or not the underlying condition value has changed. The ``continue`` keyword can be used to skip the rest of the current iteration and go on to the next one. ::

    // Prints "1 3 5 7 9"
    for i in 0..10 {
        if i % 2 == 0 {
            continue // Continue to the next iteration without printing
        }

        print(i)
    }

    loop {
        if Int.random(upperBound: 10) ==  5 {
            break // Exit the loop when the random value is equal to 5
        }
    }

Labeled Loops
-------------

CDot also supports *labeled* loops in case you need more fine-grained control over which loop you want to ``break`` or ``continue`` from. A labeled loop is introduced by writing a label name followed by a colon before the actual loop::

    // Try to figure out what this will print!
    outer: for i in 0..5 {
        for j in 0..5 {
            if i + j == 5 {
                continue outer
            }

            print("$i $j")
        }
    }

In contrast to most other languages, CDot also allows you to break out of an ``if`` statement, but only if it is explicitly labeled::

    if myCondition() {
        break // error: can't break in unlabeled if
    }

    my_if: if complexCondition() {
        if otherComplexCondition() {
            break my_if // Allowed
        }

        doSomething()
    }
