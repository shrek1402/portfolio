# strong_alias

A type-safe alternative for a typedef or a 'using' directive.

C++ currently does not support type-safe typedefs, despite multiple proposals
(ex. http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3515.pdf). The
next best thing is to try and emulate them in library code.
