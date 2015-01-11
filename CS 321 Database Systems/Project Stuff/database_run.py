#!/usr/bin/python
from Tkinter import * 
#import threading



root = Tk()

# create a menu
#menu = Menu(root)
#root.config(menu=menu)
#frame= Frame(root, width=200, height=200)
#frame.pack()
#def callback():
    #print "called the callback!"

#def pop_up():
    #l= Label(frame, text="poop")
    #l.pack(side=BOTTOM)
#filemenu = Menu(menu)
#menu.add_cascade(label="File", menu=filemenu)
#filemenu.add_command(label="New", command=callback)
#filemenu.add_command(label="Open...", command=callback)
#filemenu.add_command(label="Change label", command=pop_up)
#filemenu.add_separator()
#filemenu.add_command(label="Exit", command=callback)

#helpmenu = Menu(menu)
#menu.add_cascade(label="Help", menu=helpmenu)
#helpmenu.add_command(label="About...", command=callback)

#mainloop()


#root= Tk()



frame= Frame(root, width=500, height=200)
frame.pack()
v= StringVar()
i=0
lab= Label(root, textvariable=v)
lab.pack(side=TOP)
v.set("Foo")
def callback():
    
        v.set("poop")
        i=1
    
button= Button(frame, text="Change", command=callback)
#button.bind("<Button-1>", callback(v))
button.pack()
root.mainloop()
print i


##################
#one potential implementation
#class Tab_1(threading.Thread):
    
    #def __init__(self):
        #threading.Thread.__init__(self)
        #self.start()
        
    #def callback(self):
        #self.root.quit()
    #def run(self):
        #self.root=Tkinter.Tk()
        #self.root.protocol("WM_DELETE_WINDOW", self.callback)
        #self.s = Tkinter.StringVar()
        #self.s.set('Foo')
        #l = Tkinter.Label(self.root, textvariable=self.s)
        #l.pack()
        #self.root.mainloop()
        

    


#def main():
#app= Tab_1()

     


#main()