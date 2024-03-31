
# LOAD: {
#     scoreboard objectives add sqrt dummy
#     scoreboard players set #-2147483648 sqrt -2147483648
#     scoreboard objectives add sqrt_ast dummy
#     # The maximum effective range of <x> is the square of the number of fakeplayers
#     scoreboard players set #0 sqrt_ast 0
#     scoreboard players set #1 sqrt_ast 0
#     scoreboard players set #2 sqrt_ast 0
#     scoreboard players set #3 sqrt_ast 0
#     scoreboard players set #4 sqrt_ast 0
#     scoreboard players set #5 sqrt_ast 0
#     scoreboard players set #6 sqrt_ast 0
#     scoreboard players set #7 sqrt_ast 0
#     ...
# }

# INPUT: scoreboard players set #x sqrt <x>

scoreboard players set * sqrt_ast -2
scoreboard players operation * sqrt_ast -= * sqrt_ast
scoreboard players add * sqrt_ast 3
scoreboard players operation * sqrt_ast -= * sqrt_ast
scoreboard players operation * sqrt_ast += #x sqrt
scoreboard players operation * sqrt_ast /= #-2147483648 sqrt
scoreboard players set #y sqrt 0
scoreboard players operation #y sqrt -= * sqrt_ast

# OUTPUT: scoreboard players get #y sqrt
