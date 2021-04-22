# LBP 20-21 Group-7

## Topic
Spectre Demo using side channels

## Directory structure 

- `cache.cpp` - perform cache attack to test the cache side channel
- `spectre.cpp` - The actual spectre attack performed 


## Steps For Execution
1. Download the repository zip file and extract or clone as:-
```
    git clone https://github.com/kssuraaj28/hw-reverse.git & cd side_channnel
``` 
1.  Run the make file using the following command:-
```
    make 
```
4. Run the python script as:-
```
    ./cache
```
5. After execution is finished,reset the hugepages to 0 like:-
```
    ./spectre
``` 
### Acknowledgement
- The specific implementation for this demo of speculatve execution was taken from [Chandler Carruth's CppCon Talk.](https://www.youtube.com/watch?v=_f7O3IfIR2k)
