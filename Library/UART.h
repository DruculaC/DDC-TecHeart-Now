/*
�������� void InitUART()
���ܣ� 	��ʼ��UART
˵����
��ڲ�������
����ֵ��  ��
*/
void InitUART9600();
void InitUART1200();
void InitUART(tWord Baudrate);




/*
�������� void SendData(BYTE dat)
���ܣ� 	���ͺ���
˵����
��ڲ�����Ҫ���͵��ֽ�
����ֵ��  ��
*/
void SendData(tByte dat);




/*
��������void SendNByte(BYTE *buf,BYTE len)
���ܣ� ���Ͷ���ֽ�
˵����
��ڲ�����Ҫ���͵����ݵ�ַ�����͵��ֽڳ���
����ֵ�� ��
*/
void SendNByte(tByte *buf, tByte len);














