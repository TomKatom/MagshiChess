# Chess Game - High Level Design

### Server Side:

​	Waits for a first client, then waits for another client and then puts them in a 
​	lobby together.
​	For each lobby just forwards each client msg to the other client.

### Client Side:

​	Connects to server and waits for another player.
​	On player connection starts the game.
​	If its the players turn waits for commands from the GUI.
​	After getting a command from the GUI checks if its move or a chat command.	
​	If its a move, check if its a valid move if its not, tells the GUI of the error.
​	If the move is valid, check if after the move a check occurs check if a mate 
​	occurs and tells the GUI and the server of the mate.
​	If the move itself was valid, sends the move to the server.
​	If after the move, the player has reached the end of the board, tells the GUI to 
​	let him pick a piece and after the piece is chosen updates the board accordingly. 

​	In the background, waits for messages from the server.
​	For each message it checks whether its a chat message or a move.
​	If its a move, updates the GUI and the board accordingly, and changes the turn.
​	If its a chat message adds it to the GUI's chat,

### GUI:

​	Waits for a connection with the Client Side.
​	After a connection, starts the game.
​	On the player making a move, sends the move to Client Side and waits for 
​	approval.

​	In the background waits for chat messages and for moves coming from the 
​	other player and deals with the respectively.