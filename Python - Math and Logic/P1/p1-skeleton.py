import itertools

class Exp(object):
    """A Boolean expression.

    A Boolean expression is represented in terms of a *reserved symbol* (a
    string) and a list of *subexpressions* (instances of the class `Exp`).
    The reserved symbol is a unique name for the specific type of
    expression that an instance of the class represents. For example, the
    constant `True` uses the reserved symbol `1`, and logical and uses `∧`
    (the Unicode symbol for conjunction). The reserved symbol for a
    variable is its name, such as `x` or `y`.

    Attributes:
        sym: The reserved symbol of the expression (a string).
        sexps: The list of subexpressions (instances of the class `Exp`).
    """

    def __init__(self, sym, *sexps):
        """Constructs a new expression.

        Args:
            sym: The reserved symbol for this expression.
            sexps: The list of subexpressions.
        """
        self.sym = sym
        self.sexps = sexps

    def value(self, assignment):
        """Returns the value of this expression under the specified truth
        assignment.

        Args:
            assignment: A truth assignment, represented as a dictionary
            that maps variable names to truth values.

        Returns:
            The value of this expression under the specified truth
            assignment: either `True` or `False`.
        """
        raise ValueError()

    def variables(self):
        """Returns the (names of the) variables in this expression.

        Returns:
           The names of the variables in this expression, as a set.
        """
        variables = set()
        for sexp in self.sexps:
            variables |= sexp.variables()
        return variables

class Var(Exp):
    """A variable."""

    def __init__(self, sym):
        super().__init__(sym)

    def value(self, assignment):
        assert len(self.sexps) == 0
        return assignment[self.sym]

    def variables(self):
        assert len(self.sexps) == 0
        return {self.sym}

class Nega(Exp):
    """Logical not."""
    def __init__(self, sexp):
        super().__init__('¬', sexp)

    def value(self, assignment):
        assert len(self.sexps) == 1
        return not self.sexps[0].value(assignment)
    # TODO: Complete this class

class Conj(Exp):
    """Logical and."""

    def __init__(self, sexp1, sexp2):
        super().__init__('∧', sexp1, sexp2)

    def value(self, assignment):
        assert len(self.sexps) == 2
        return \
            self.sexps[0].value(assignment) and \
            self.sexps[1].value(assignment)

class Disj(Exp):
    """Logical or."""
    def __init__(self, sexp1, sexp2):
        super().__init__('∨', sexp1, sexp2)

    def value(self, assignment):
        assert len(self.sexps) == 2
        return \
            self.sexps[0].value(assignment) or \
            self.sexps[1].value(assignment)
    # TODO: Complete this class

class Impl(Exp):
    """Logical implication."""
    def __init__(self, sexp1, sexp2):
        super().__init__('→', sexp1, sexp2)

    def value(self, assignment):
        assert len(self.sexps) == 2
        #According to implication rules,
        #1 0 means it should return false, all other cases true
        return not (self.sexps[0].value(assignment) and \
                not self.sexps[1].value(assignment) )
    # TODO: Complete this class

class Equi(Exp):
    """Logical equivalence."""
    def __init__(self, sexp1, sexp2):
        super().__init__('↔', sexp1, sexp2)

    def value(self, assignment):
        assert len(self.sexps) == 2
        #According to equivalence rules,
        #If either both are true or both are false (they are equivalent to each other),
        #then return true
        return self.sexps[0].value(assignment) ==\
               self.sexps[1].value(assignment)
    # TODO: Complete this class

def assignments(variables):
    """Yields all truth assignments to the specified variables.

    Args:
        variables: A set of variable names.

    Yields:
        All truth assignments to the specified variables. A truth
        assignment is represented as a dictionary mapping variable names to
        truth values. Example:

        {'x': True, 'y': False}
    """
    # TODO: Complete this function. Use the itertools module!
    assignments = []
    #Get possible combination of truth values based on amount of variables
    combinations = itertools.product(range(2), repeat = len(variables))
    for comb in combinations:
        assignment = {}
        for i, truth_val in enumerate(comb):
            #Key is symbol of variable and turth_val is truth value found in current combination values
            #Convert to list to be able to get value from "set" by indexing
            assignment[list(variables)[i]] = bool(truth_val)
        #Add a truth assignment to list of truth assignments
        assignments.append(assignment)
    return assignments


def satisfiable(exp):
    """Tests whether the specified expression is satisfiable.

    An expression is satisfiable if there is a truth assignment to its
    variables that makes the expression evaluate to true.

    Args:
        exp: A Boolean expression.

    Returns:
        A truth assignment that makes the specified expression evaluate to
        true, or False in case there does not exist such an assignment.
        A truth assignment is represented as a dictionary mapping variable
        names to truth values.
    """
    # TODO: Complete this function
    #Get possible combination
    for assign in assignments(exp.variables()):
        #Expression is satisfiable if any truth assignment gives True        
        if (exp.value(assign)):
            return True
    return False

def tautology(exp):
    """Tests whether the specified expression is a tautology.

    An expression is a tautology if it evaluates to true under all
    truth assignments to its variables.

    Args:
        exp: A Boolean expression.

    Returns:
        True if the specified expression is a tautology, False otherwise.
    """
    # TODO: Complete this function
    for assign in assignments(exp.variables()):
        #Expression is not a tautology if any truth assignment doesn't give True
        if (not exp.value(assign)):
            return False
    return True

def equivalent(exp1, exp2):
    """Tests whether the specified expressions are equivalent.

    Two expressions are equivalent if they have the same truth value under
    each truth assignment.

    Args:
        exp1: A Boolean expression.
        exp2: A Boolean expression.

    Returns:
        True if the specified expressions are equivalent, False otherwise.
    """
    # TODO: Complete this function

    #NOTE: Both expressions have same amount of variables which means
    #you can use expression's variables    
    for assign in assignments(exp1.variables()):
        #If the expression assigned to same values doesn't have the same result, return False
        #"Is not didn't work because expression 2 returns integer?"
        if (exp1.value(assign) != exp2.value(assign)):
            return False
    return True

def test_satisfiable1():
    a = Var('a')
    #¬a
    exp = Nega(a)
    return satisfiable(exp)

def test_satisfiable2():
    a = Var('a')
    b = Var('b')
    #a->b
    exp = Impl(a, b)
    return satisfiable(exp)

def test_satisfiable3():
    a = Var('a')
    #a ∧ ¬a
    exp = Conj(a, Nega(a))
    return satisfiable(exp)

def test_tautology1():
    a = Var('a')
    #a v ¬a
    exp = Disj(a, Nega(a))
    return tautology(exp)

def test_tautology2():
    a = Var('a')
    b = Var('b')
    #a ∧ b
    exp = Conj(a, Nega(a))
    return tautology(exp)

def test_equivalent1():
    a = Var('a')
    b = Var('b')
    c = Var('c')
    # (a->b)->c
    exp1 = Impl(Impl(a, b), c)
    # (a v c) ∧ (¬b ∨ c)
    exp2 = Conj(Disj(a, c), Disj(Nega(b), c))
    return equivalent(exp1, exp2)

def test_equivalent2():
    a = Var('a')
    b = Var('b')
    #a ∧ b
    exp1 = Conj(a, b)
    exp2 = Conj(a, b)
    return equivalent(exp1, exp2)

if __name__ == "__main__":
    # TODO: Add some more test cases
    print()
    print("Test satisfiable 1:")
    print(str(test_satisfiable1()) + "-->should be True")

    print()
    print("Test satisfiable 2:")
    print(str(test_satisfiable2()) + "-->should be True")

    print()
    print("Test satisfiable 3:")
    print(str(test_satisfiable3()) + "-->should be False")

    print()
    print("Test tautology 1:")
    print(str(test_tautology1()) + "-->should be True")

    print()
    print("Test tautology 2:")
    print(str(test_tautology2()) + "-->should be False")

    print()
    print("Test equivalent 1:")
    print(str(test_equivalent1()) + "-->should be ???")

    print()
    print("Test equivalent 2:")
    print(str(test_equivalent1()) + "-->should be True")