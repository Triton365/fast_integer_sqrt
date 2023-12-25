# LOAD: {
#     scoreboard objectives add sqrt dummy
#     scoreboard players set #2 sqrt 2
#     scoreboard players set #559 sqrt 559
#     scoreboard players set #32768 sqrt 32768
# }

# INPUT: scoreboard players set #x sqrt <x>

execute store result score #t1 sqrt store result score #t2 sqrt store result score #t3 sqrt run scoreboard players operation #y sqrt = #x sqrt
execute if score #x sqrt matches 0..1515359 run scoreboard players operation #y sqrt /= #559 sqrt
execute if score #x sqrt matches 0..1515359 run scoreboard players add #y sqrt 15
execute if score #x sqrt matches 1515360.. run scoreboard players operation #y sqrt /= #32768 sqrt
execute if score #x sqrt matches 1515360.. run scoreboard players add #y sqrt 2456
scoreboard players operation #t1 sqrt /= #y sqrt
scoreboard players operation #y sqrt += #t1 sqrt
scoreboard players operation #y sqrt /= #2 sqrt
scoreboard players operation #t2 sqrt /= #y sqrt
scoreboard players operation #y sqrt += #t2 sqrt
scoreboard players operation #y sqrt /= #2 sqrt
scoreboard players operation #t3 sqrt /= #y sqrt
scoreboard players operation #y sqrt += #t3 sqrt
scoreboard players operation #y sqrt /= #2 sqrt
scoreboard players operation #x sqrt /= #y sqrt
execute if score #y sqrt > #x sqrt run scoreboard players remove #y sqrt 1

# OUTPUT: scoreboard players get #y sqrt