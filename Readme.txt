
		 ReconDroid (Reconnaissance Droid) 

			Version 0.2


	This is the program to browse, connect to and launch servers
for Jedi Knight and Mysteries of Sith and Outlaws (all the other 
DirectPlay are also sort of supported - you can browse and
connect to the servers if the game supports DirectPlayLobby
interface correctly).


				Notes

Getting started:

	When you start the program you see three panes - Server Lists,
Servers and Active Servers. "Server list" pane contains sources where the 
server lists can be obtained from. That can be your own lists or lists 
retrieved from WWW sites like JK.Net or DarkJedi. The "Servers" pane shows 
the content of the current selected list. The "Active servers" pane shows
servers that were polled and found to be running games.

	In order to join a game, you must get it to display in "Active
Servers" pane. To do that, you need to obtain some server lists first,
then poll them to find out if they are running any games. The simpliest
way is to pick the menu item "Update and Poll all Lists" in "Game" menu.
It'll retrieve the server list from all lists in the "Server Lists" pane, 
poll them and present you with the list of active ones (if any). This 
may take a while. You can do it selectively, by selecting the only list/
list group you want and picking "Update and Poll servers" (or double-click). 
Note that when you select, say, a list group "Jedi Knight", all the lists 
that belong to it (JK.Net lists, DarkJedi list, and your custom list in 
standard tree) will be updated and polled. You can also poll individual 
servers from the list. Select them and pick "Poll Servers", or double-click
on them. However since polling is very quick now it's simplier and easier to
just poll the whole list.

	Once you got some servers to display in "Active Servers" window, you
can decide which game to join. The game name, level played, number of players
and ping are shown for each game. To get more information you can select a 
server and pick "Properties" in "Active Servers" menu (or right click and select
"Properties"). The dialog box will appear that contains additional information about 
the game - player names, whether the game has time/score limit, etc. You can double
click on a server to update it. And select "Connect" in "Active Servers" menu or
"Join Game" in "Game" menu to join it. JK (or MOTS) will be started and
automatically connected to the selected server. Just in case, if it fails,
the program also copies the IP of the server to the clipboard so you can 
paste it to the "Connect" dialog.

	To start your own game, pick "Host Game" in "Game" menu. Fill in
the stuff and click "Host". The program will automatically post the
information to JK.Net and/or DarkJedi IP boards for you (if you select 
the appropriate checkboxes) and start JK/MOTS for you. You'll just need
to make sure you host a game with the parameters you specified.


			Added in version 0.2

	Now when you try to join a a game ReconDroid checks if you have the level that is
played on the server. If you don't, it offers you to download and unzip it before joining.
You can set the "Add-on" directory when ReconDroid will put the levels it downloads. Use
"Options" menu to set them. By default (or if you specify an empty value for the directory),
ReconDroid stores them in Episode\ directory. You might want to change that.

	ReconDroid uses the file sites.lst and game's comment to look for download sites
for a specific level. So, if you are hosting a game using some uncommon level, leave an
URL where it can be downloaded in game's comment.

	Also, ReconDroid now supports Outlaws. Outlaws games can be played in two ways -
via DirectPlay or via Winsock. ReconDroid supports both. Winsock is a recommened way to do
this, as ReconDroid can retrieve more information about a game played this way.


				Advanced

	Here's some features of the program that might not be immediately
needed, but might become quite useful later:
	
	If you feel like it, you can edit the tree that appears in 
"Server Lists" pane. It is contained in the file "lists.dat", which is
a plain text file of the following format:

Each node of the tree is defined by a constuct

GROUP group_name
 group parameters 
ENDGROUP

These constructs can be nested, letting you make branches. The folwoing
group paramers are supported now:

url=value
format=value

"url" value can be either:

- a path to a local file, full or relative to RecondDroid directory.
- an http://server/[path/file] - standard HTTP link
- a reserved word "ipx:" - for a list of local IPX games

"format" specifies the format of data that the give url contains. It 
can be one of the following:

jknet		- JK.net IP board format
darkjedi	- DarkJedi list format
text		- plain text format of local server lists
ripips		- ReconDroid will rip IP addresses from the page/file.


	Also, sometimes you might want to edit/update the ReconDroid download site database.
It is contained in file "sites.lst" and the format is trivial:

gob/goo_name url

For instance:

CogKing.gob			http://www.jediknight.net/files_2/CogKing1.zip
speeder.goo			ftp://ftp.gamesnet.net/pub/jediknight/jk/levels/mp/dr.zip
"The Arena.gob"		http://www.jediknight.net/files_2/arena_2.zip

You can have comments, preceeded by # character. Note that if the name of the GOB/GOO
has space in it, you must use quotes. http:// and ftp:// URLs are accepted. You can 
have multiple entries for each GOB/GOO. ReconDroid will present you with the list of them all. 
Also, it is a good idea to include the URL in game's comment in case you're playing some level 
that isn't in ReconDroid database. ReconDroid scans the game's comment for http:// and 
ftp:// URLs and adds them to the list of download sites it presents to you.

	Another thing about auto-downloading. By default the "Add-on directory", i.e. - the
directory where the levels ReconDroid downloads will be extracted to, is set to Episode\ 
directory of JK/MOTs respectively - the directory where your original game levels are stored. 
You might want to change that to a directory of your choice to avoid filling your Episode\ 
directory with files. There's only one gotcha though - if you want to host games using a 
custom level you will have to copy copy/move its GOB/GOO file to Episode\ directory. But if 
you just need to join games running this levels, you don't need to.


Another useful feature of ReconDroid is you can use hypertext links
to make it poll a specific server. To do that, use this link format

rdroid://ip

For instance, put this on your page

<A HREF="rdroid://1.1.1.1"> 1.1.1.1</A>

And everyone who has ReconDroid installed can just click on that link to
make ReconDroid poll the specified server.



Have fun online,
Alex.