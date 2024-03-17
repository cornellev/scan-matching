# Copyright (C) 2024 Ethan Uppal. All rights reserved.

from sympy import *

init_printing(use_unicode=True)

t, o_x, o_y, c_x, c_y, t_x, t_y, b_x, b_y = symbols("t o_x o_y c_x c_y t_x t_y b_x b_y")
a_x = cos(t) * o_x - sin(t) * o_y + c_x
a_y = sin(t) * o_x + cos(t) * o_y + c_y
L = (a_x + t_x - b_x) ** 2 + (a_y + t_y - b_y) ** 2

print("L=")
print(simplify(expand(L)))
print()

print("dL/dt_x=")
print(simplify(expand(diff(L, t_x))))
print()

print("dL/dt_y=")
print(simplify(expand(diff(L, t_y))))
print()

print("dL/dtheta=")
print(
    "("
    + str(simplify(expand(diff(L, t)) / 2))
    .replace("sin(t)", "std::sin(t.theta)")
    .replace("cos(t)", "std::cos(t.theta)")
    .replace("c_x", "a_cm_x")
    .replace("c_y", "a_cm_y")
    .replace("t_x", "t.dx")
    .replace("t_y", "t.dy")
    + ") * 2 / n"
)
