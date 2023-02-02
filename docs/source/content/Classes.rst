
.. _classes:

Classes
=======

Classes in CDot are similar to structs, but have one very important difference in their ownership behaviour: structs are *value types*, while classes are *reference types*. This means that a value of struct type needs to be explicitly copied or moved if you need to store it in more than one place, while a value of class type can be arbitrarily copied and referenced from multiple places, with each reference pointing to the same value. Classes are *reference counted*, which means that they keep track of how many references to the value exist, and take care of cleaning up themselves once that value reaches zero. The following example may make the difference between classes and structs more clear::

    struct S with ImplicitlyCopyable {
        var value: Int
    }

    class C {
        var value: Int
    }

    var s = S(value: 1)
    var s2 = s // OK because S is ImplicitlyCopyable

    s2.value = 2
    print(s.value) // Prints "1"

    var c = C(value: 1)
    var c2 = c // Copying a class just creates a new reference

    c2.value = 2
    print(c1.value) // Prints "2"

As you can see, ``s2`` refers to a completely different value than ``s``, even though it was created via a direct reference to ``s``. On the other hand, both ``c`` and ``c2`` refer to the same underlying value, which means that modifications to ``c2`` are also reflected on ``c``.

Inheritance
-----------

Because classes are reference types, they also allow for *inheritance*, a feature which you are probably familiar with if you've used another object oriented programming language before. You can define an inheritance clause with a colon following the class name::

    abstract class Animal {
        def abstract makeSound()
    }

    class Cat: Animal {
        def override makeSound() {
            print("meow")
        }
    }

    class Dog: Animal {
        def override makeSound() {
            print("woof")
        }
    }

    def useAnimal(_ animal: Animal) {
        animal.makeSound()
    }

    useAnimal(Cat()) // Prints "meow"
    useAnimal(Dog()) // Prints "woof"

As you can see, classes also allow for the declaration of *virtual* (and *abstract*) methods. These will dynamically dispatch to the most specific implementation at runtime. An abstract method cannot define a body and must be overriden by all subclasses, while a virtual definition *may* or *may not* be overriden.
