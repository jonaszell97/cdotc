
.. highlight:: CDot
   :linenothreshold: 3

.. _custom-operators:

Custom Operators
================

CDot allows you to define custom overloads of the available operators, as well as entirely new ones. Operators are declared like normal functions, but with a special syntax for the function name.

Overloading Existing Operators
------------------------------

Existing operators like ``+`` or ``-`` can be implemented for new types easily. Simply define a global function with the operator name in place of a normal function name. For an infix function, the declaration needs to take exactly two parameters, for a prefix or postfix function, only one parameter is allowed. ::

    /// Remove the right-hand side string from the left one. 
    def infix -(_ str: String, _ rem: String) -> String {
        if str.endsWith(rem) {
            return str[0..(str.size - rem.size)]
        }

        return str
    }

    let s1 = "Hello, World!"
    print(s1 - ", World!") // Prints "Hello"

Defining New Operators
----------------------

If you want to define an operator that does not yet exist in the language, you need to declare it first. An operator declaration starts with one of the keywords ``infix``, ``prefix``, or ``postfix``, followed by the operator name and an optional *precedence group*. Suppose we want to create the familiar increment and decrement operators from ``C``. First, we have to declare the operators themselves::

    prefix ++
    postfix ++
    prefix --
    postfix --

Once we've done that, we can define our custom operator functions::

    def prefix ++(_ value: mut ref Int) -> Int {
        value += 1
        return value
    }

    def postfix ++(_ value: mut ref Int) -> Int {
        var oldVal = value
        value += 1
        return oldVal
    }

    // And so on...

.. note::
    You may have noticed the ``mut ref`` before the parameter type in the function signatures. This is required because this function needs to *mutate* the parameter it is passed. You can learn more about this in a :ref:`different chapter <ownership>`.

Infix operators can also define a *precedence group*, which will decide how the operator will be parsed. You may remember from school that multiplication has a higher *precedence* than addition, which is reflected by the following *precedence group* declarations in the standard library::

    precedenceGroup AdditionPrecedence {
        associativity: left
        higherThan: // Some other precedence group
    }

    precedenceGroup MultiplicationPrecedence {
        associativity: left
        higherThan: AdditionPrecedence
    }

    // Now we can declare the operators like so...
    infix + : AdditionPrecedence
    infix * : MultiplicationPrecedence

Precedence groups are related to each other with ``higherThan`` and ``lowerThan`` relationships. If an operator belongs to a precedence group that is *higher* than another, it will be parsed first, which means that the following two expressions are equivalent::

    _ = 3 + 4 * 5
    _ = 3 + (4 * 5) // equivalent - `*` binds more tightly than `+`

If no relationship is established between two precedence groups, you are required to use parentheses to disambiguate.

.. note::
    There are two more attributes you can define for a precedence group besides ``higherThan`` and ``lowerThan``. The first is the ``associativity``, which can be either ``left`` or ``right`` (default is ``left``). The associativity of an operator specifies how a sequence of operators with the same precedence level are grouped together in the absence of grouping parentheses. For example, the ``-`` operator is left-associative, which means that the expression ``3 - 4 - 5`` is equivalent to ``(3 - 4) - 5``, which equals -12. If it were right associative, it would be parsed as ``3 - (4 - 5)``, which would equal 4. The second is ``assignment``, which can be ``true`` or ``false`` (default is ``false``).
