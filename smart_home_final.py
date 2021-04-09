import serial
import tkinter as tk
import time
import tkinter.font as tkFont

ser = serial.Serial('/dev/cu.usbmodem141101',baudrate = 9600,timeout = 1)
time.sleep(3)


window = tk.Tk()
window.title('Smart Home Controller')
window.geometry('500x350')

Button = tk.Button
Label = tk.Label
font1 = tkFont.Font(size=16)


def on1():
    ser.write(b'a')
    time.sleep(0.5)


def on2():
    ser.write(b'f')
    time.sleep(0.5)


def on3():
    ser.write(b'l')
    time.sleep(0.5)


def on4():
    ser.write(b'x')
    time.sleep(0.5)


def on5():
    ser.write(b'y')
    time.sleep(0.5)


def on6():
    ser.write(b'z')
    time.sleep(0.5)



    
air = Label(window,text="Air-con",bg="yellow",width=7,font=font1)
fan = Label(window,text="Fan",bg="yellow",width=7,font=font1)
lig = Label(window,text="Light",bg="yellow",width=7,font=font1)
b1 = Button(window,text="ON",bg="white",width=8,command=on1)
b2 = Button(window,text="ON",bg="white",width=8,command=on2)
b3 = Button(window,text="ON",bg="white",width=8,command=on3)
b4 = Button(window,text="OFF",bg="white",width=8,command=on4)
b5 = Button(window,text="OFF",bg="white",width=8,command=on5)
b6 = Button(window,text="OFF",bg="white",width=8,command=on6)


b1.grid(row=1,column=0,ipadx=20,padx=20,ipady=10,pady=15)
b2.grid(row=1,column=1,ipadx=20,padx=20,ipady=10,pady=15)
b3.grid(row=1,column=2,ipadx=20,padx=20,ipady=10,pady=15)
b4.grid(row=2,column=0,ipadx=20,padx=20,ipady=10,pady=15)
b5.grid(row=2,column=1,ipadx=20,padx=20,ipady=10,pady=15)
b6.grid(row=2,column=2,ipadx=20,padx=20,ipady=10,pady=15)
air.grid(row=0,column=0,ipadx=20,padx=20,ipady=10,pady=15)
fan.grid(row=0,column=1,ipadx=20,padx=20,ipady=10,pady=15)
lig.grid(row=0,column=2,ipadx=20,padx=20,ipady=10,pady=15)

# Kill programme
def kill_programme():
    ser.write(b"p")
    window.destroy()
    exit()


Button(window, text="Exit", width=8, command=kill_programme).grid(row=3,column=1,ipadx=20,padx=20,ipady=10,pady=15)

window.mainloop()



    
        
    
         
        
    


    
