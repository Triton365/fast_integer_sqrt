from math import isqrt


f = open('lut_load.mcfunction','w')
fwrite = f.write
lut = []


# lut[0] = 1
fwrite('data modify storage sqrt: lut set value [\\\n    1')
lut.append(1)
assert len(lut) == 1


# lut[1~467] = 0
for _ in range(1,468):
    fwrite(',0')
    lut.append(0)
fwrite('\\\n   ')
assert len(lut) == 468


# lut[468~1048576] = 980~46340
DIV = 2048
xmax = 960511 - DIV
while xmax <= 2147483647:
    xmax += DIV
    xmaxsqrt = isqrt(xmax)
    fwrite(',%d'%xmaxsqrt)
    lut.append(xmaxsqrt)
fwrite('\\\n   ')
assert len(lut) == 1048577


# lut[-239615~-1] = 979~?
xmax = 958463 + 4
while xmax >= 8:
    xmax -= 4
    xmaxsqrt = isqrt(xmax)
    fwrite(',%d'%xmaxsqrt)
    lut.append(xmaxsqrt)
fwrite('\\\n]')
print(len(lut))
f.close()


def sqrt_lut(x):
    if x <= 958463: y = lut[-(x//4)]
    if x >= 958464: y = lut[x//2048]
    #if score #x sqrt matches ..958463 store result storage sqrt: i int -.25 run scoreboard players get #x sqrt
    #if score #x sqrt matches 958464.. store result storage sqrt: i int .00048828125 run scoreboard players get #x sqrt
    x //= y
    if y > x: return y-1
    return y

