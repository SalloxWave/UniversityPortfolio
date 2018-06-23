#!/usr/bin/env python3
# -*- utf-8 -*-

#Link to online calculator
#http://xrjunque.nom.es/rootfinder.aspx


# TDP015 Inl�mningsuppgift 5

# Er uppgift �r att implementera Newtons metod f�r att approximera nollst�llen
# till en funktion och att sedan till�mpa denna implementation f�r att l�sa
# tre numeriska ber�kningsproblem. En beskrivning av metoden hittar du h�r:
#
# https://sv.wikipedia.org/wiki/Newtons_metod
#
# Metoden presenteras �ven p� f�rel�sningen v. 18.

# ## Del 1

# ### Problem 1
#
# Implementera ett steg av Newton-approximationen.

def newton_one(f, f_prime, x0):
    """Compute the next Newton approximation to a root of the function `f`,
    based on an initial guess `x0`.

    Args:
        f: A float-valued function.
        f_prime: The function's derivative.
        x0: An initial guess for a root of the function.

    Returns:
        The next Newton approximation to a root of the function `f`, based on
        the initial guess `x0`.
    """
    #Use newton's formula
    x1 = x0 - ( f(x0)/f_prime(x0) )
    return x1

# ### Problem 2
#
# Implementera en fullst�ndig Newton-approximation.

def newton(f, f_prime, x0, n_digits=6):
    """Compute the Newton approximation to a root of the function `f`, based
    on an initial guess `x0`.

    Args:
        f: A float-valued function.
        f_prime: The function's derivative.
        x0: An initial guess for a root of the function.
        n_digits: The desired precision of the approximation.

    Returns:
        A pair consisting of the Newton approximation to a root of the
        function `f` and the number of iterations needed to reach numerical
        stability. The precision of the approximation should be the specified
        number `n_digits` of digits after the decimal place. That is, the
        iterative process should end when the approximation does not alter the
        first `n_digits` after the decimal place.
    """
    old_x = 0.0
    current_x = x0
    diff = 1.0
    iterations = 0
    #Loop as long as difference between the values is not zero
    #(with specified number of digits)
    while diff != 0:
        iterations+=1
        #Save old and get next approximation
        old_x = current_x
        current_x = newton_one(f, f_prime, current_x)
        
        #Count difference between current and old value with specified amount of digits
        diff = round(current_x, n_digits) - round(old_x, n_digits)
    return current_x, iterations

# ## Del 2

# ### Problem 3
#
# Anv�nd din kod f�r att approximera nollst�llet av funktionen
#
# f(x) = x^3 - x + 1
#
# Ange Python-funktioner `f` och `f_prime` och ett l�mpligt startv�rde `x0`
# och skriv ut ert numeriska resultat inklusive antalet iterationer.
# Noggrannheten ska vara sex siffror efter kommat.

def problem3():
    f = lambda x: x**3 - x + 1.0
    f_prime = lambda x: 3 * x**2 - 1.0
    x0 = 1.2
    a, i = newton(f, f_prime, x0)
    #Result according to website (see link top): -1.32471795724475
    print("approximation: {:.6f}, number of iterations: {}".format(a, i))

# ### Problem 4
#
# Anv�nd er kod f�r att approximativt ber�kna den femte roten ur 5. B�rja med
# att ange den funktion vars nollst�lle ni vill approximera. Forts�tt sedan
# som i f�reg�ende uppgift.

def problem4():
    #f(x) = x^5 - 5
    f = lambda x: x**5 - 5.0
    f_prime = lambda x: 4 * x**4
    x0 = 1.5
    a, i = newton(f, f_prime, x0)
    print("approximation: {:.6f}, number of iterations: {}".format(a, i))

# ### Problem 5
#
# Ange en funktion och tv� olika startv�rden s�dana att Newtons metod r�knar
# ut tv� helt olika approximationer.

def problem5():
    f = lambda x: x**2 - 2
    f_prime = lambda x: 2*x
    x01 = 1.0
    x02 = -1.0
    a1, _ = newton(f, f_prime, x01)
    a2, _ = newton(f, f_prime, x02)
    print("approximation 1: {:.6f}".format(a1))
    print("approximation 2: {:.6f}".format(a2))


if __name__ == "__main__":
    print("Problem 3")
    problem3()

    print("")
    print("Problem 4")
    problem4()

    print("")
    print("Problem 5")
    problem5()