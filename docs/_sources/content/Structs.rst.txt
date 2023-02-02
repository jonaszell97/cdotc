
Structs
=======

CDot provides a variety of different ways to define custom types, the simplest of which are *structure types*, or ``struct`` s for short. In fact, most of the types we have been using in the previous sections like Int and String are defined as structs in the standard library. Structs allow you to group related data and functionality in a common place. You define a struct by writing the ``struct`` keyword followed by a name. After that, you can define the struct's contents in curly braces.

Fields
------

A struct declaration can contain one or more *stored properties*, or *fields*, which are introduced with the familiar ``var`` and ``let`` keywords. The fields that you declare on the struct are what determines its physical size and layout, every instance of that struct will have enough space to store all of its fields. Let's say we want to define a ``struct`` to store information about a person, such as his or her name, age, and address. We could declare these three as *fields* on a struct::

    struct Person {
        // The name may change, so make it a variable
        var name: String

        // Birthday can't change, make it constant
        let birthday: String

        // Address can change, so declare a mutable field
        var address: String
    }

Every person we create will now have storage for these three fields. Because we did not write an initializer, CDot implicitly created one for us - you can learn more about this in :ref:`initialization`. This means that we can create a new person like this::

    var mike = Person(name: "Mike", birthday: "02/29/1997", address: "22 Broadway")
    print(mike.name) // Prints "Mike"

    mike.address = "23 Broadway" // OK - address was declared as a var
    mike.name = "Peter" // error - reassignment rules also apply to structs

Stored fields can optionally define a *default value*, which will be assigned to the field if no value is given. For example, if we want to allow persons to exist without an address for privacy reasons, we could define the address field like this::

    var address: String = ""

This allows us to create a person without supplying an address::

    var kate = Person(name: "Kate", birthday: "05/12/1989")

Properties
----------

CDot provides a second type of property you can define on custom types, called *computed properties* or simply *properties*, which are introduced with the ``prop`` keyword. A computed property differs from a stored one in that there is no physical storage allocated for it in the struct - the value it represents is computed every time you access it. This computation is done in *accessor methods*, which you define after the property declaration. There are two types of accessor methods: *getters* and *setters*. The *setter* method is called whenever the property is referenced on the left hand side of an assignment, in all other cases, the *getter* is called. You have to define at least a getter or a setter for every property. Suppose we want to allow users of the person class to query a person's age, which we can compute using the stored birthday::

    prop age: Int {
        get {
            // Assume that getCurrentDate is a function that returns the current date
            return (getCurrentDate() - birthday).years
        }
    }

    print(kate.age) // Prints "31"

If we wanted to allow modifying the birthday via the age property as well, we could define a *setter*::

    prop age: Int {
        get { ... } // Same as above
        set {
            // The new value is accessible via an implicit ``newVal`` parameter
            birthday = getCurrentDate().subtract(years: newVal)
        }
    }

    kate.age = 19
    print(kate.birthday) // Prints "01/01/2001"

.. note::
    There is an additional set of accessors besides ``get`` and ``set``, the *modification accessors* ``read`` and ``write``. These accessors make a property feel more like a stored value by allowing you to take a mutable reference to the property, which will then be written back at the end of the reference's lifetime.

Shorthand Property Syntax
-------------------------

If a property only has a getter accessor, you can leave out the ``get`` keyword and put the body directly in the braces. If the return value is a single expression, you can even leave out the body and ``return`` entirely and just use a double arrow::

    // Short syntax
    prop age: Int {
        return (getCurrentDate() - birthday).years
    }

    // Even shorter syntax
    prop age: Int => (getCurrentDate() - birthday).years

Methods
-------

Structs can also contain *methods*, which are functions that logically belong to the type. Every method has an implicit ``self`` parameter which is set to the struct instance on which the method was called. For example, we may want to allow our person to be greeted, which we could do by implementing a method::

    struct Person {
        // Like above
        // ...

        def greet() {
            print("Hello, ${self.name}!")
        }

        // Mutating methods can modify ``self``
        def mutating rename(to name: String) {
            self.name = name
        }
    }

    // Call method greet on the instance mike of type Person
    mike.greet() // Prints "Hello, Mike!"

Defining and calling methods follows the same rules as functions with regards to labeling, default values, and so on, which the exception that a method has to be called on an instance of a type. By default, the ``self`` value cannot be modified inside of a method, unless you declare it as ``mutating``.

.. note::
    Property setter methods are implicitly marked as ``mutating``. You can opt out of this behaviour with the contextual ``nonmutating`` keyword.

Subscripts
----------

Subscripts are a special kind of accessor that you have already encountered in previous examples. They allow you to use the familiar element access syntax using square brackets ``[]`` on any value. Subscripts are declared using the ``subscript`` keyword followed by a parameter list. Since subscripts behave more like functions than properties, the type of the subscript is declared using a single arrow. The definition of the getters and setters for the subscript is similar to a computed property declaration. ::

    struct Household {
        /// The people in this Household.
        var people: [Person]

        /// Allow access to a person of this household via their name.
        subscript (name: String) -> Person? {
            get {
                for p in people {
                    if p.name == name {
                        return p
                    }
                }

                return none
            }
        }
    }

    var household = new Household(people: [mike, kate]
    print(household["Kate"]) // Prints "Person { name = "Kate", birthday = "05/12/1989" }"
    print(household["Sarah"]) // Prints "none"

    household["Sarah"] = Person(...) // error: subscript does not define a setter
