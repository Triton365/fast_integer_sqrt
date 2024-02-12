from math import isqrt


# lut[0] = 1
lut = [1]


# lut[1~467] = 0
lut += [0]*467
assert len(lut) == 468


# lut[468~1048576] = 980~46340
DIV = 2048
xmax = 960511 - DIV
while xmax <= 2147483647:
    xmax += DIV
    xmaxsqrt = isqrt(xmax)
    lut.append(xmaxsqrt)
assert len(lut) == 1048577


# lut[-239615~-1] = 979~?
xmax = 958463 + 4
while xmax >= 8:
    xmax -= 4
    xmaxsqrt = isqrt(xmax)
    lut.append(xmaxsqrt)
print(len(lut))


def sqrt_lut(x):
    if x <= 958463: y = lut[-(x//4)]
    if x >= 958464: y = lut[x//2048]
    #if score #x sqrt matches ..958463 store result storage sqrt: i int -.25 run scoreboard players get #x sqrt
    #if score #x sqrt matches 958464.. store result storage sqrt: i int .00048828125 run scoreboard players get #x sqrt
    x //= y
    if y > x: return y-1
    return y


with open('lut_load.mcfunction','w') as f:
    firstline = True
    doublebracket = False

    string = 'data modify storage sqrt: lut set value [' + str(lut[0])
    length = len(string)
    f.write(string)
    for x in lut[1:]:
        string = ','+str(x)
        length += len(string)
        if length >= 1999999:
            if firstline:
                string = ']\ndata modify storage sqrt: temp set value [[' + str(x)
                firstline = False
                doublebracket = True
            elif doublebracket:
                string = ']]\ndata modify storage sqrt: temp append value [' + str(x)
                doublebracket = False
            else:
                string = ']\ndata modify storage sqrt: temp append value [' + str(x)
            length = len(string)
        f.write(string)
    if doublebracket:
        f.write(']')
    f.write(']\ndata modify storage sqrt: lut append from storage sqrt: temp[][]')
    f.write('\ndata remove storage sqrt: temp')
    f.write('\nscoreboard objectives add sqrt dummy')
