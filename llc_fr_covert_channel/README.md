# LBP 20-21 Group-7

## Topic
Implementation of LLC Flush+Reload Covert channel

## Directory structure 

- `sender.c` - The first attacker that will try to send messages to receiver using flush-reload covert channel
- `receiver.c` - The second attacker that will try to read signals from the sender without the knowledge of OS
- `shared.txt` - A shared file that helps in communication between two processes.
- `util.h` - Some helpful utilities


## Steps For Execution


1. To compile the code run  
```console
	make
```
2. Configuring the covert-channel
You have to modify the config macros in `util.h` and rebuild

3. Running the covert-channel\\
To test the channel, simply run on two different terminals:-
```
	./sender 
```
```
	./receiver
``` 

## Acknowledgement
The specific implementation (synchronous sender-receiver protocol) of this covert channel is adapted from [<span style="color:blue"><ins>here</ins></span>](https://github.com/ricpacca/deaddrop) 
