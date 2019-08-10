main            ::= expr.

expr            ::= IDENTIFIER.
expr            ::= literal.
expr            ::= procedure_call.
expr            ::= lambda.

literal         ::= NUMBER.

procedure_call  ::= PAREN_L operator operands PAREN_R.

operator        ::= expr.
operands        ::= operands expr.
operands        ::= .

lambda          ::= PAREN_L LAMBDA PAREN_L formals PAREN_R body PAREN_R.

formals         ::= formals IDENTIFIER.
formals         ::= .

body            ::= expr.
