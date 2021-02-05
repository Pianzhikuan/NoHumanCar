import serial
import serial.tools.list_ports  #导入模块
from time import sleep
from tkinter import *
from uart_test import recv,sandMsg

#一.获取串口 or 判断是否存在该串口
# def getSerial(needdev=''):
#     needSerial=''
#     port_list = list(serial.tools.list_ports.comports())
#     if len(port_list) <= 0:
#         print ("The Serial port can't find!")
#     else:
#     #1.过滤，获取需要的串口
#         for i in list(port_list):
#             if i.name==needdev:
#                 needSerial=i.name
#             print(i.name,end=',')
#         print('\n')
#     #2.返回获取的目的串口为了绑定
#     return needSerial
#由于传输10对应ASCII中的回车，会有问题，为此增加11的偏移
offset = 11
#索引代表的意思：0~2-帧头 2~7-下，x，y，z  8-帧尾
ReturnToZero = [0x3D, 0x5A, offset, offset, offset, offset, offset, offset, 0x0A]  #刹车##A5 5A代表发送给机械臂
move = [0x3D, 0x5A, 0, 0, 0, 0, 0, 0, 0x0A]#初始化


def numberINIT(index1, index2, num):
    '''  用于数据move数据处理，初始化 '''
    #不能太大。需要有个限制
    if num >= 0:
        if num > 2000:
            num = 2000
        move[index1] = num // 100 + offset  # 高位
        move[index2] = num % 100 + offset  # 低2位
    else:
        num = abs(num)
        if num > 2000:
            num = 2000
        move[index1] = num // 100 + offset   # 高位
        move[index2] = num % 100 + offset  # 低2位 
        move[index1] = move[index1] | (1 << 6)  #第7位也就是最高位置为1，表示负数//由于chr的原因，不能取最高位


def printInfo():
    ''' 用串口发送信息 '''
    x = int(float(X_axisE.get()))
    y = int(float(Y_axisE.get()))
    z = int(float(Z_axisE.get()))
    print("X:%dmm \Y: %dmm \Z: %dmm" % (x, y, z))
    numberINIT(2, 3, x)
    numberINIT(4, 5, y)
    numberINIT(6, 7, z)

    sandMsg(ser, move)


''' 获取可用串口列表：'''
choose = input("是否查看可用串口列表(运行速度会慢很多)是输入1，否输入0:\n")
if choose == '1':
    port_list = list(serial.tools.list_ports.comports())
    print(port_list)
    if len(port_list) == 0:
        print('无可用串口')
    else:
        for i in range(0,len(port_list)):
            print(port_list[i])
print("--------------------------------------------")
# portx = input('请输入要链接的串口(如COM10/com10):\n')
print("当前默认端口号为com15")
print("--------------------------------------------")
#bps = input('请设置波特率：\n')
print("当前默认波特率为9600")
print("--------------------------------------------")

'''创建一个串口'''
try:
    #端口，GNU / Linux上的/ dev / ttyUSB0 等 或 Windows上的 COM3 等
    portx='com15'
    #波特率，标准值之一：50,75,110,134,150,200,300,600,1200,1800,2400,4800,9600,19200,38400,57600,115200
    bps=9600
    #超时设置,None：永远等待操作，0为立即返回请求结果，其他值为等待超时时间(单位为秒）
    timex=5
    # 打开串口，并得到串口对象
    ser = serial.Serial(portx, bps, timeout=timex)
    
    #sandMsg(ser,'前进')

except Exception as e:
    print("open failed")
    print("---异常---：", e)
else:
    if ser.isOpen() :
        print("open success")

#运行到这里说明串口打开成功

#--------------------GUI界面的创建-------------------------#
#--------------------窗口创建------------------------------#
root = Tk()
root.title("PC端小车远程控制窗口")
screenWidth = root.winfo_screenwidth()
screenHeight = root.winfo_screenheight()
w = 400
h = 550
x = (screenWidth - w) / 2
y = (screenHeight - h) / 2 - 50
root.geometry("%dx%d+%d+%d" % (w, h, x, y))
root.resizable(False, False) #设置为不能更改宽和高

msg = "欢迎使用PC小车远程控制GUI系统"
logo = Label(root,text=msg)
#--------------------小车控制相关按钮创建-----------------------------------------------------------------------------#
#forwardBtn = Button(root,Text='前进',command=lambda : (sandMsg(ser,'前进')))
bt_w = 10
bt_h = 2
btn_Forward = Button(root, text="前进", width=bt_w, height=bt_h, command=lambda: (sandMsg(ser, '前进')))
btn_Left = Button(root, text="左转", width=bt_w, height=bt_h, command=lambda: (sandMsg(ser, '左转')))
btn_Back = Button(root, text="后退", width=bt_w, height=bt_h, command=lambda: (sandMsg(ser, '后退')))
btn_Right = Button(root, text="右转", width=bt_w, height=bt_h, command=lambda: (sandMsg(ser, '右转')))
btn_Stop = Button(root, text="刹车", width=bt_w, height=bt_h, command=lambda: (sandMsg(ser, '刹车')))
btn_Catch = Button(root, text="抓取", width=bt_w, height=bt_h, command=lambda: (sandMsg(ser, '抓取')))
btn_Put = Button(root, text="松开", width=bt_w, height=bt_h, command=lambda: (sandMsg(ser, '松开')))

#--------------------机械臂控制相关文本输入框Entry和按钮创建-----------------------------------------------------------------------------#

X_axisL = Label(root,text="X轴坐标/mm")         # X标签
X_axisL.grid(row=4)                        # 默认是 column=0
Y_axisL = Label(root,text="Y轴坐标/mm")        # Y标签
Y_axisL.grid(row=5)  # 默认是 column=0
Z_axisL = Label(root,text="Z轴坐标/mm")         # Z标签
Z_axisL.grid(row=6)                        # 默认是 column=0

X_axisE = Entry(root)             # X文本框
Y_axisE = Entry(root)             # Y文本框
Z_axisE = Entry(root)             # Z文本框

ReturnToZeroBtn = Button(root, text="归零(ready)", width=bt_w, height=bt_h, command=lambda: (sandMsg(ser, ReturnToZero),print("X:%dmm \Y: %dmm \Z: %dmm" % (0, 0, 0))))
ReturnToZeroBtn.grid(row=7,column=0)
moveBtn = Button(root,text="MOVE(getted)",width=bt_w, height=bt_h,command=printInfo)
moveBtn.grid(row=7,column=1)
quitBtn = Button(root,text="Quit",width=bt_w, height=bt_h,command=root.quit)
quitBtn.grid(row=7, column=2)
quitBtn.config(fg="red",bg="yellow")

#////////////控件位置设置位/////////////////////////////#
# btn_Forward.place(x=100,y=2)
# btn_Left.place(x=0,y=60)
# btn_Right.place(x=200,y=60)
# btn_Back.place(x=100, y=113)
logo.grid(row=0,column=0,columnspan=2,pady=10,padx=20)
X_axisE.grid(row=4,column=1,pady=20)                   # 定位X文本框
Y_axisE.grid(row=5, column=1, pady=10)                 # 定位Y文本框
Z_axisE.grid(row=6, column=1, pady=10)                 # 定位Z文本框

btn_Stop.config(fg="red",bg="yellow")

btn_Forward.grid(row=1, column=1, pady=5, padx=5)
btn_Left.grid(row=2, column=0, pady=5, padx=5)
btn_Right.grid(row=2, column=2, pady=5, padx=5)
btn_Back.grid(row=3, column=1, pady=5, padx=5)
btn_Stop.grid(row=2, column=1)
btn_Catch.grid(row=8, column=0, pady=5, padx=5)
btn_Put.grid(row=8, column=1, pady=5, padx=5)

# btn_Forward.pack(side=TOP)
# btn_Left.pack(side=LEFT)
# btn_Right.pack(side=RIGHT)
# btn_Back.pack(side=BOTTOM)


root.mainloop()#卡在这里，让程序继续执行，放在程序最后一行
ser.close()#关闭串口