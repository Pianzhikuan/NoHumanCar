from tkinter import *

#--------------------GUI界面的创建-------------------------#
#--------------------窗口创建------------------------------#
root = Tk()
root.title("PC端小车远程控制窗口")
screenWidth = root.winfo_screenwidth()
screenHeight = root.winfo_screenheight()
w = 600
h = 400
x = (screenWidth - w) / 2
y = (screenHeight - h) / 2
root.geometry("%dx%d+%d+%d" % (w, h, x, y))
#--------------------按钮创建-------------------------#
#forwardBtn = Button(root,Text='前进',command=lambda : (sandMsg(ser,'前进')))
bt_w = 10
bt_h = 2
btn_Forward = Button(root, text="前进", width=bt_w, height=bt_h, command=lambda : (sandMsg(ser,'前进')))
btn_Left = Button(root, text="左转", width=bt_w, height=bt_h, command=lambda : (sandMsg(ser,'左转')))
btn_Back = Button(root, text="后退", width=bt_w, height=bt_h, command=lambda : (sandMsg(ser,'后退')))
btn_Right = Button(root, text="右转", width=bt_w, height=bt_h, command=lambda : (sandMsg(ser,'右转')))

#--------------------机械臂控制相关文本输入框Entry和按钮创建-----------------------------------------------------------------------------#
msg = "Welcome to Python GUI tkinter Learning System!!!"
logo = Label(root,text=msg)

accountL = Label(root,text="Account")         # name标签
accountL.grid(row=4)                        # 默认是 column=0
pwdL = Label(root,text="Password")    # address标签
pwdL.grid(row=5)                     # 默认是 column=0


logo.grid(row=0,column=0,columnspan=2,pady=10,padx=10)
accountE = Entry(root)                      # name文本框
pwdE = Entry(root,show="*")                   # address文本框
accountE.grid(row=4,column=1,pady=20)               # 定位name文本框
pwdE.grid(row=5,column=1,pady=10)            # 定位address文本框

# 以下建立Login和Quit按钮
# loginbtn = Button(root,text="Login",command=printInfo)
# loginbtn.grid(row=3,column=0)
# quitbtn = Button(root,text="Quit",command=root.quit)
# quitbtn.grid(row=3,column=1)






btn_Forward.grid(row=1,column=1,pady=5,padx=5)
btn_Left.grid(row=2,column=0,pady=5,padx=5)
btn_Right.grid(row=2,column=2,pady=5,padx=5)
btn_Back.grid(row=3,column=1,pady=5,padx=5)
# btn_Forward.pack(side=TOP)
# btn_Left.pack(side=LEFT)
# btn_Right.pack(side=RIGHT)
# btn_Back.pack(side=BOTTOM)
# btn_Forward.place(x=100,y=2)
# btn_Left.place(x=0,y=60)
# btn_Right.place(x=200,y=60)
# btn_Back.place(x=100,y=113)

root.mainloop()#卡在这里，让程序继续执行，放在程序最后一行
#ser.close()#关闭串口