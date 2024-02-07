from time import sleep
import subprocess
from pynput.keyboard import Key, KeyCode, Listener, Controller

# https://pynput.readthedocs.io/en/latest/keyboard.html

keyboard=Controller()

ctrlIsPressed=False

def changeTags():
    print("changing tags...")
    keyboard.type("s\\{(.*)\\{");
    keyboard.tap(Key.enter)
    keyboard.tap(Key.left);
    keyboard.type("T{c");
    #TODO make it give you a mnu with all the tags, make the ability to remove & add tags
    #instead of just resetting them


    
def changeDocument():
    print("changing the document linked...")
    keyboard.type("s\\{(.*)\\{");
    keyboard.tap(Key.enter)
    keyboard.tap(Key.right);

    choice=subprocess.getoutput("cat documents-list.txt | dmenu -p '' -c -bw 5 -l 5 -i")
    if len(choice)==0:
        return
    
    keyboard.type("t}c")
    keyboard.type(choice)
    keyboard.tap(Key.esc)

def changeShownName():
    print("changing the displayed name...")
    keyboard.type("s\\}(.*)\\}");
    keyboard.tap(Key.enter)
    keyboard.tap(Key.left);
    keyboard.type("T}c");


def doAction(key):
    
    global ctrlIsPressed

    #perform actions
    if key==Key.enter and ctrlIsPressed:
        ctrlIsPressed=False;
        
        print("start")
        #unpress enter and go to normal mode
        keyboard.release(Key.enter)
        keyboard.tap(Key.backspace) # backspace in case we are in insert mode; we wanna bring the line back up
        keyboard.tap(Key.esc) # in case we are in insert mode, go back to normal mode
        keyboard.type("mammam")

        choice=subprocess.getoutput("echo 'tags\ndoc\nname' | dmenu -c -bw 5 -l 5 -i -n")
        keyboard.release(Key.enter)
        keyboard.release('t')
        keyboard.release('d')
        keyboard.release('n')

        if(len(choice)==0):
            return True
        
        match choice[0]:
            case 't':
                changeTags()

            case 'd':
                changeDocument()

            case 'n':
                changeShownName()

            case _:
                print("wut")
                
        print("done")
            

        ctrlIsPressed=True;
        
    #exit                
    elif key==KeyCode(char='q'):
        return False
        
    #ctrl mod is pressed
    elif key==Key.ctrl:
        ctrlIsPressed=True
    
def undoAction(key):
    
    global ctrlIsPressed

    #control mod is released
    if key==Key.ctrl:
        ctrlIsPressed=False


with Listener(on_press=doAction, on_release=undoAction) as listener:  
    listener.join()



