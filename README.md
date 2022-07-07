# strong_alias

* Этот репозиторий не является рабочим кодом и не содержит в себе все нужные заголовочные файлы. 
Он нужен только для того, чтобы показать некоторые части кода, чтобы интервьюер и/или работодатель видел, как и какой код я пишу.

Type-safe альтернатива для 'using'.

C++ в настоящее время не поддерживает типобезопасные typedefs, несмотря на многочисленные предложения
(например. http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3515.pdf ).

MI strong_alias пытается эмулировать их.

```// using Orange = int;
// using Apple = int;
// Apple apple(2);
// Orange orange = apple;      // Orange should not be able to become an Apple.
// Orange x = orange + apple;  // Shouldn't add Oranges and Apples.
// if (orange > apple);        // Shouldn't compare Apples to Oranges.
// void foo(Orange);
// void foo(Apple);            // Redefinition.
// etc.
//
// StrongAlias may instead be used as follows:
//
// using Orange = StrongAlias<class OrangeTag, int>;
// using Apple = StrongAlias<class AppleTag, int>;
// using Banana = StrongAlias<class BananaTag, std::string>;
// Apple apple(2);
// Banana banana("Hello");
// Orange orange = apple;                              // Does not compile.
// Orange other_orange = orange;                       // Compiles, types match.
// Orange x = orange + apple;                          // Does not compile.
// Orange y = Orange(orange.value() + apple.value());  // Compiles.
// Orange z = Orange(banana->size() + *other_orange);  // Compiles.
// if (orange > apple);                                // Does not compile.
// if (orange > other_orange);                         // Compiles.
// void foo(Orange);
// void foo(Apple);                                    // Compiles into separate overload.```
