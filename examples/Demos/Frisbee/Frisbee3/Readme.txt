Hi there !

Ok, I decided to start a small game, and release different versions of it for you to understand several tricks...

This first demo is simple : a frisbee, rotating, goes around the screen, from one screen to another. The important stuff in the sources :
- Have the frisbee sprite loaded on both screens... Duh !
- Added a SCREENHOLE definition, put it to 48... on emulator it'll look weird, since the screens are next to one another, but it looks perfect on DS. You can try other sizes (32 wasn't good)
- Try to get the fixed point clear in your head... Fixed point is used instead of float, because it's way faster...
- And look at the trick to position the sprites from one screen to another, it's really simple !


Have fun for now, I'll update this demo when I work on it a little more :p

Mollusk