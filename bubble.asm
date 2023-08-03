; outline of bubble sort program
ldc 0x1000 
a2sp 
adj -1 
ldc 10 
stl 0 
ldc array
call sort
HALT 
; 
; Areg= return address 
; Breg= address of elements 
; SP+0= number of elements sort
; int* sort(int* in,int count)
;{
;    int x;
;    int y;
;    int temp;
;    for(x=0;x < count-1;x++)
;    {
;        for(y=1;y<count-x;y++)
;        {
;            if((*(in+y))>(*(in+y-1)))
;            {
;                temp=(*(in+y-1));
;                (*(in+y-1))=(*(in+y));
;                (*(in+y))=temp;
;            }
;        }
;    }
;    exit:return;
;} 
sort: adj -1
;a=return address,b=address of elements
stl 0
;save return address, a=address of elements, b=address of elements now
stl 2
;save address of elements, a=address of elements, b=address of elements now
adj -3
ldc 0
;a=0,b=address of elements
stl 0
;save temp
ldc 1
stl 1
;save y,a=address of elements, b=address of elements now 
ldc 0
stl 2
;save x, ,a=address of elements, b=address of elements now
loop_out: ldc array
stl 5
ldl 4
;          0  1 2        3           4    5 
;SP,mem: temp y x (return address) count  address of array
;load count
ldl 2
;load x, b=count, a=x now
sub
;a=count - x b=count
ldc 1
;a=1 b=count-x
sub
;a=count-1-x, b=count-x
brz done
ldc 1
stl 1
;a=count-1-x, b=count-1-x
;y = 1
loop_in: ldl 4
;load count, a=count,b=count-1-x
ldl 2
;load x, b=count, a=x
sub
;a=count - x,b=x
ldl 1
;load y,a=y,b=count-x 
sub 
;a=count - x -y,b=count-x
brz addx
ldl 5
;load address of elements, b=count-x-y, a=adress of array
ldnl 0
;a=array[0],b=count-x-y
ldl 5
;save array[0] to Breg,a=address of array,b=array[0]
ldnl 1
;array[1],a=array[1],b=array[0]
sub
;array[0]-array[1]
brlz swap
br addy
swap: ldl 5
;load address of elements,a=address of elements, b=array[0]-array[1]
ldnl 0
;a=array[0],b=arr[0]-arr[1]
stl 0
;temp = array[0],a=arr[0]-arr[1], b=arr[0]-arra[1]
ldl 5
ldnl 1
;a=array[1],b=array[0]-array[1]
ldl 5
;a=address of array,b=arr[1]
stnl 0
;array[0] = array[1]
ldl 0
;load temp, a=temp,b=address of array
ldl 5
stnl 1
;array[1] = temp
;increment
addy: ldc 1
ldl 1
;load y
add
;y++
stl 1
;save y++
ldc 1
ldl 5
add
;array++
stl 5
br loop_in
addx: ldc 1
ldl 2
;load x 
add
;x++
stl 2
br loop_out
done: ldl 3
;load return address
adj 5
return
;ok, 
array: data 2
data 10
data 3
data 4
data 5
data 6
data 7
data 8
data 9
data 1
