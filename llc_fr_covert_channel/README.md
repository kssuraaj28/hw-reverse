#Implementation of LLC Flush+Reload Covert channel

## To compile the code run  
`make`

## Configuring the covert-channel
You have to modify the config macros in util.h and rebuild

## Running the covert-channel
To test the channel, simply run `./sender` and `./receiver` on two terminals.

## Acknowledgement
The specific implementation (synchronous sender-receiver protocol) of this covert channel is adapted from https://github.com/ricpacca/deaddrop) 
