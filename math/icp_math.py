import sympy as sp

Point = sp.Matrix
Matrix = sp.Matrix

class RigidBodyTransform:
    def __init__(self):
        theta, tx, ty = sp.symbols('theta t_x t_y')
        self.theta = theta
        self.rotation = Matrix([[sp.cos(theta), -sp.sin(theta)], [sp.sin(theta), sp.cos(theta)]])
        self.translation = Point([tx, ty])
    
    def apply_to(self, point: Point, pivot: Point):
        return self.rotation * (point - pivot) + pivot + self.translation

class Match:
    def __init__(self):
        ax, ay, bx, by = sp.symbols('a_x a_y b_x b_y')
        self.a = Point([ax, ay])
        self.b = Point([bx, by])
        
    def cost_with(self, rbt: RigidBodyTransform, pivot: Point):
        v = rbt.apply_to(self.a, pivot) - self.b - pivot
        return v[0] ** 2 + v[1] ** 2

def make_centroid():
    cx, cy = sp.symbols('c_x c_y')
    return Point([cx, cy])

def latexify(f):
    return str(f) \
        .replace('theta', '\\theta') \
        .replace('cos', '\\cos') \
        .replace('sin', '\\sin') \
        .replace('a_x - c_x', '{a\'}_x') \
        .replace('a_y - c_y', '{a\'}_y') \
        .replace('c_x', '\\bar{a}_x') \
        .replace('c_y', '\\bar{a}_y') \
        .replace('*', '') 
            

rbt = RigidBodyTransform()
pair = Match()
pivot = make_centroid()

cost = pair.cost_with(rbt, pivot)

print(sp.simplify(cost))

# print(latexify(sp.diff(cost, rbt.translation[0])))
# print(latexify(sp.diff(cost, rbt.translation[1])))
# print(latexify(sp.diff(cost, rbt.theta)))

