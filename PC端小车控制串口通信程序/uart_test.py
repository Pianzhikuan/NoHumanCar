import serial  #导入模块
from time import sleep

Forward = (0x5A, 0x3D, 0x06, 0x83, 0x00, 0x00, 0x01, 0x00, 0x0A)  #前进
Left = (0x5A, 0x3D, 0x06, 0x83, 0x00, 0x00, 0x01, 0x01, 0x0A)  #左转
Back = (0x5A, 0x3D, 0x06, 0x83, 0x00, 0x00, 0x01, 0x02, 0x0A)  #后退
Right = (0x5A, 0x3D, 0x06, 0x83, 0x00, 0x00, 0x01, 0x03, 0x0A)  #右转
Stop = (0x5A, 0x3D, 0x06, 0x83, 0x00, 0x00, 0x01, 0x04, 0x0A)  #刹车
Catch = (0x5A, 0x3D, 0x06, 0x83, 0x00, 0x00, 0x00, 0x50, 0x0A)  #抓取
Put = (0x5A, 0x3D, 0x06, 0x83, 0x00, 0x00, 0x00, 0x0F, 0x0A)  #松开
offset = 11

def recv(serial):
    while True:
        data = serial.readline().decode("gbk")
        if data == '':
            continue
        else:
            break
        sleep(0.02)
    return data

#发送数据--参数：ser-串口   msg-要发送的数据
def sandMsg(ser,msg):
    if msg == '前进':
        for i in range(0, len(Forward)):
            ser.write(chr(Forward[i]).encode("utf-8"))
        print('前进')
    elif msg == '左转':
        for i in range(0, len(Left)):
            ser.write(chr(Left[i]).encode("utf-8"))
        print('左转')
    elif msg == '后退':
        for i in range(0, len(Back)):
            ser.write(chr(Back[i]).encode("utf-8"))
        print('后退')
    elif msg == '右转':
        for i in range(0, len(Right)):
            ser.write(chr(Right[i]).encode("utf-8"))
        print('右转')
    elif msg == '刹车':
        for i in range(0, len(Right)):
            ser.write(chr(Stop[i]).encode("utf-8"))
        print('刹车')
    elif msg == '抓取':
        for i in range(0, len(Catch)):
            ser.write(chr(Catch[i]).encode("utf-8"))
        print('抓取')
    elif msg == '松开':
        for i in range(0, len(Put)):
            ser.write(chr(Put[i]).encode("utf-8"))
        print('松开')
    elif isinstance(msg, list):  #是列表list
        # ser.write(bytes(msg))  #这个也可以
        for i in range(0, len(msg)):
            # print("%d " % (msg[i]-offset), end=' ')
            ser.write(chr(msg[i]).encode("utf-8"))

    
        
        

#-----------------------测试用主函数----------------------#
if __name__ == '__main__':
    '''创建一个串口'''
    try:
        #端口，GNU / Linux上的/ dev / ttyUSB0 等 或 Windows上的 COM3 等
        portx="COM15"
        #波特率，标准值之一：50,75,110,134,150,200,300,600,1200,1800,2400,4800,9600,19200,38400,57600,115200
        bps=115200
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
    
    #---------------接收函数------------#
    while True:
        data =recv(ser)
        if data != b'' :
            print("receive : ",data)
            if data == b'x':
                print("exit")
                break

    ser.close()#关闭串口


# # 写数据(中文的发送)
# result=ser.write("我是东小东".encode("gbk"))
# print("写总字节数:", result)
# #print("串口详情参数：", ser)

# ''' 获取可用串口列表：'''
# port_list = list(serial.tools.list_ports.comports())
# print(port_list)
# if len(port_list) == 0:
#    print('无可用串口')
# else:
#     for i in range(0,len(port_list)):
#         print(port_list[i])

'''十六进制处理：'''
# #十六进制的发送
# result=ser.write(chr(0x06).encode("utf-8"))#写数据
# print("写总字节数:",result)

# #十六进制的读取
# print(ser.read().hex())  #读一个字节

# print(ser.port)#获取到当前打开的串口名
# print(ser.baudrate)#获取波特率

# result=ser.write("我是东小东".encode("gbk"))#写数据
# print("写总字节数:",result)


# #print(ser.read())#读一个字节
# # print(ser.read(10).decode("gbk"))#读十个字节
# #print(ser.readline().decode("gbk"))#读一行
# #print(ser.readlines())#读取多行，返回列表，必须匹配超时（timeout)使用
# #print(ser.in_waiting)#获取输入缓冲区的剩余字节数
# #print(ser.out_waiting)#获取输出缓冲区的字节数

# #循环接收数据，此为死循环，可用线程实现
# while True:
#         if ser.in_waiting:
#             str=ser.read(ser.in_waiting ).decode("gbk")
#             if(str=="exit"):#退出标志
#                 break
#             else:
#             print("收到数据：",str)

# print("---------------")
# ser.close()#关闭串口



