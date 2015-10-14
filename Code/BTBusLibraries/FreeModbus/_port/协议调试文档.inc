1、设置参数设置
软件：Modbus Poll - Mbpoll1
菜单：Read/Write Definition(F8)
参数：Slave ID		1
      Function		16 Write Multiple Register
      Address		0
      Quantity		8
      ScanRate		1000ms
      Read/WriteEnabled False
数据：Alias		0(Display->Hex)
      0			ADDRESS0_HW_LOW
      1			ADDRESS0_HW_HIGH
      2			ADDRESS1_HW_LOW
      3			ADDRESS1_HW_HIGH
      4			ADDRESS2_HW_LOW
      5			ADDRESS2_HW_HIGH
      6			ADDRESS3_HW_LOW
      7			ADDRESS3_HW_HIGH
2、读取参数设置
软件：Modbus Poll - Mbpoll1
菜单：Read/Write Definition(F8)
参数：Slave ID		1
      Function		03 Read Holding Register (4x)
      Address		0
      Quantity		8
      ScanRate		1000ms
      Read/WriteEnabled False
数据：Alias		0(Display->Hex)
      0			ADDRESS0_HW_LOW
      1			ADDRESS0_HW_HIGH
      2			ADDRESS1_HW_LOW
      3			ADDRESS1_HW_HIGH
      4			ADDRESS2_HW_LOW
      5			ADDRESS2_HW_HIGH
      6			ADDRESS3_HW_LOW
      7			ADDRESS3_HW_HIGH
3、读取数据
软件：Modbus Poll - Mbpoll1
菜单：Read/Write Definition(F8)
参数：Slave ID		1
      Function		04 Read Input Register (3x)
      Address		0
      Quantity		8
      ScanRate		1000ms
      Read/WriteEnabled False
数据：Alias		0(Display->Hex)
      0			[温度数据]
      1			[电压|序列/信号值]
      2			[温度数据]
      3			[电压|序列/信号值]
      4			[温度数据]
      5			[电压|序列/信号值]
      6			[温度数据]
      7			[电压|序列/信号值]