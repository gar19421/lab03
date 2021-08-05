#Codigo para comunicacion serial GUI lab03
#Brandon Garrido -19421
#Digital 2 seccion 20


#LIBRERÍAS
from tkinter import *
from tkinter.font import Font
from time import sleep 
import serial
import time
import sys

import tkinter as tk



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
vent.geometry('400x400')


def button1():#CUANDO SE PRESIONA EL BOTON DE CONECTAR
    global vent
    var1 = []
    n=0
    flag = False
    flag1=False
    while(flag == False):
        var = puerto.read().decode('ascii')
        if var == '\r':
            flag = True
        if (flag):
            while(n<9):
                var = puerto.read().decode('ascii')
                var_temp = var
                var1.append(var_temp)    
                n = n+1
                flag = False
                flag1 = True
    #RENDERIZAR VALORES DE USART A INTERFAZ
    label_pots1=tk.Label(vent, text=var1[0])
    label_pots1.place(x=120, y=250)
    label_pots2=tk.Label(vent, text=var1[1])
    label_pots2.place(x=140, y=250)
    label_pots3=tk.Label(vent, text=var1[2])
    label_pots3.place(x=160, y=250)
    label_pots4=tk.Label(vent, text=var1[3])
    label_pots4.place(x=180, y=250)
    label_pots5=tk.Label(vent, text=var1[4])
    label_pots5.place(x=200, y=250)
    label_pots6=tk.Label(vent, text=var1[5])
    label_pots6.place(x=220, y=250)
    label_pots7=tk.Label(vent, text=var1[6])
    label_pots7.place(x=240, y=250)
    label_pots7=tk.Label(vent, text=var1[7])
    label_pots7.place(x=260, y=250)
    label_pots7=tk.Label(vent, text=var1[8])
    label_pots7.place(x=280, y=250)


def button3():#CUANDO SE PRESIONA EL BOTON DE ENVIAR DATOS A CONTADOR
    global vent
    
    mystring=tex.get(1.0,END)
    b = mystring.encode('utf-8')

    puerto.write(b)
        

    tex.delete(1.0,END)
    tex.insert(1.0,"")

    

#CREACIÓN DE BOTONES

l1=Button(vent,text= 'Conectar', command=button1 ,cursor='arrow')
l3=Button(vent,text='ENVIAR CONTADOR',command=button3,cursor='arrow')

#CREACION DE ETIQUETAS 
lab1=Label(vent,text='   ',width=15,height=3)
lb2=Label(vent,text='               GUI LAB 3',width=38,height=4)
#CREACION DE TEXTO

pot1= tk.Label(vent, text="POT1:")
pot1.place(x=150, y=230)
pot2 = tk.Label(vent, text="POT2:")
pot2.place(x=240, y=230)



tex= Text(vent,width=20,height=2)

#ESTABLECER POSICIONES DE LOS BOTONES EN LA INTERFAZ GRAFICA
l1.place(x=180,y=200)
l3.place(x=150,y=300)

#ESTABLECER POSICIONES DE LAS ETQIUETAS Y TEXTOS EN LA INTERFAZ GRAFICA
lab1.place(x=150,y=100)
lb2.place(x=48,y=40)
tex.place(x=125,y=140)
vent.mainloop()



    







#CERRAR EL PUERTO SERIAL
puerto.close()
print('PUERTO BLOQUEADO')
sys.exit(0)