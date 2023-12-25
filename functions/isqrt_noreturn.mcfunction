# LOAD: {
#     scoreboard objectives add sqrt dummy
#     scoreboard players set #2 sqrt 2
#     scoreboard players set #141 sqrt 141
#     scoreboard players set #7775 sqrt 7775
#     scoreboard players set #100000 sqrt 100000
# }

# INPUT: scoreboard players set #x sqrt <x>

execute store result score #t2 sqrt run scoreboard players operation #t1 sqrt = #x sqrt
execute if score #x sqrt matches 0..19310 run function {
    scoreboard players add #t1 sqrt 4095
    scoreboard players set #y sqrt -594039
    scoreboard players operation #y sqrt /= #t1 sqrt
    scoreboard players add #y sqrt 149
}
execute if score #x sqrt matches 19311..1705544 run function {
    scoreboard players add #t1 sqrt 903167
    scoreboard players set #y sqrt -1568669636
    scoreboard players operation #y sqrt /= #t1 sqrt
    scoreboard players add #y sqrt 1857
}
execute if score #x sqrt matches 1705545..39400514 run function {
    scoreboard players operation #t1 sqrt /= #141 sqrt
    scoreboard players add #t1 sqrt 224767
    scoreboard players set #y sqrt -2147483648
    scoreboard players operation #y sqrt /= #t1 sqrt
    scoreboard players add #y sqrt 10425
}
execute if score #x sqrt matches 39400515..455779650 run function {
    scoreboard players operation #t1 sqrt /= #7775 sqrt
    scoreboard players add #t1 sqrt 60419
    scoreboard players set #y sqrt -2147483648
    scoreboard players operation #y sqrt /= #t1 sqrt
    scoreboard players add #y sqrt 39184
}
execute if score #x sqrt matches 455779651..2147483647 run function {
    scoreboard players operation #t1 sqrt /= #100000 sqrt
    scoreboard players add #t1 sqrt 26111
    scoreboard players set #y sqrt -2147483648
    scoreboard players operation #y sqrt /= #t1 sqrt
    scoreboard players add #y sqrt 91181
}
scoreboard players operation #t2 sqrt /= #y sqrt
scoreboard players operation #y sqrt += #t2 sqrt
scoreboard players operation #y sqrt /= #2 sqrt
scoreboard players operation #x sqrt /= #y sqrt
execute if score #y sqrt > #x sqrt run scoreboard players remove #y sqrt 1

# OUTPUT: scoreboard players get #y sqrt