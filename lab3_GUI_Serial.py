from tkinter import *
from tkinter.font import Font
from time import sleep 
import serial
import time
import sys

var1 = "POT1: ";
var2 = "POT2: "
#CREAR PUERTO SERIAL PARA LA COMUNICACION  USANDO PYTHON
puerto= serial.Serial()
#DEFINIR VELOCIDAD EN BAUDIOS
puerto.baudrate=9600
puerto.timeout=3#tiempo hasta recibir un caracterer de fin de linea
#DEFINIR PUERTO COM DEL FTDI232
puerto.port='COM3'
#ABRIR UNA CONEXION SERIAL
puerto.open()
print('PUERTO SERIAL LISTO PARA LA COMUNICAICON')
#Crear un objeto Tk() 
vent=Tk()
#TITULO DE LA INTERFAZ
vent.title("     COMUNICACION SERIAL Y SPI     ")
#DIMENSIONES DE LA INTERFAZ
vent.geometry('400x300')


def button3():#CUANDO SE PRESIONA EL BOTON DE "LED BLINK"
    global vent
    
    mystring=tex.get(1.0,END)
    b = mystring.encode('utf-8')

    puerto.write(b)
        

    tex.delete(1.0,END)
    tex.insert(1.0,"")

    

#CREACIÓN DE BOTONES
l1=Button(vent,text= var1,cursor='arrow')
l2=Button(vent,text= var2 ,cursor='arrow')
l3=Button(vent,text='ENVIAR CONTADOR',command=button3,cursor='arrow')

#CREACION DE ETIQUETAS 
lab1=Label(vent,text='   ',width=15,height=3)
lb2=Label(vent,text='               GUI LAB 3',width=38,height=4)
#CREACION DE TEXTO
tex= Text(vent,width=20,height=2)

#ESTABLECER POSICIONES DE LOS BOTONES EN LA INTERFAZ GRAFICA
l1.place(x=130,y=200)
l2.place(x=220,y=200)
l3.place(x=160,y=250)

#ESTABLECER POSICIONES DE LAS ETQIUETAS Y TEXTOS EN LA INTERFAZ GRAFICA
lab1.place(x=150,y=100)
lb2.place(x=48,y=40)
tex.place(x=125,y=140)
vent.mainloop()



my_list = ''
    

#dividir valores del potenciometro
while (1):
         
            #angulo = ser.readline()
            #ventana.posicion_garra(angulo)
          
            try:
                
                datosASCII = puerto.read() #Devuelve b
            
               

                print(datosASCII)


                
            except :
                pass







#CERRAR EL PUERTO SERIAL
puerto.close()
print('PUERTO BLOQUEADO')
sys.exit(0)