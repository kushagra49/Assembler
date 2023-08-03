; gives a%b
call gcd
HALT


mod:
ldc ra
stnl 0

loop:
    ldc a
    ldnl 0
    ldc b
    ldnl 0
    sub

    brlz end
    ldc a
    stnl 0
    br loop

end:
ldc a
ldnl 0

ldc ra
ldnl 0
return

gcd:

; assume x is smaller
loopg:
    ldc x
    ldnl 0
    brz endg

  ldc b
  stnl 0

  ldc y
  ldnl 0
  ldc a
  stnl 0

  call mod

  ldc x
  ldnl 0
  ldc y
  stnl 0

  ldc a
  ldnl 0
  ldc x
  stnl 0

  br loopg

endg:
ldc y
ldnl 0

a: data 10
b: data 3
ra: data 0
x: data 75
y: data 200
