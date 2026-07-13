#include "bsp_iic.h"


/******************************************************************
 * 函 数 名 称：IIC_Start
 * 函 数 说 明：IIC起始时序
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void IIC_Start(void)
{
        SDA_OUT();
        SCL(1);
        SDA(0);

        SDA(1);
        delay_us(5);
        SDA(0);
        delay_us(5);

        SCL(0);
}
/******************************************************************
 * 函 数 名 称：IIC_Stop
 * 函 数 说 明：IIC停止信号
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void IIC_Stop(void)
{
        SDA_OUT();
        SCL(0);
        SDA(0);

        SCL(1);
        delay_us(5);
        SDA(1);
        delay_us(5);

}

/******************************************************************
 * 函 数 名 称：IIC_Send_Ack
 * 函 数 说 明：主机发送应答或者非应答信号
 * 函 数 形 参：0发送应答  1发送非应答
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void IIC_Send_Ack(unsigned char ack)
{
        SDA_OUT();
        SCL(0);
        SDA(0);
        delay_us(5);
        if(!ack) SDA(0);
        else     SDA(1);
        SCL(1);
        delay_us(5);
        SCL(0);
        SDA(1);
}


/******************************************************************
 * 函 数 名 称：I2C_WaitAck
 * 函 数 说 明：等待从机应答
 * 函 数 形 参：无
 * 函 数 返 回：0有应答  1超时无应答
 * 作       者：LC
 * 备       注：无
******************************************************************/
unsigned char I2C_WaitAck(void)
{

        char ack = 0;
        unsigned char ack_flag = 10;
        SCL(0);
        SDA(1);
        SDA_IN();

        SCL(1);
        while( (SDA_GET()==1) && ( ack_flag ) )
        {
                ack_flag--;
                delay_us(5);
        }

        if( ack_flag <= 0 )
        {
                IIC_Stop();
                return 1;
        }
        else
        {
                SCL(0);
                SDA_OUT();
        }
        return ack;
}

/******************************************************************
 * 函 数 名 称：Send_Byte
 * 函 数 说 明：写入一个字节
 * 函 数 形 参：dat要写人的数据
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void Send_Byte(uint8_t dat)
{
        int i = 0;
        SDA_OUT();
        SCL(0);//拉低时钟开始数据传输

        for( i = 0; i < 8; i++ )
        {
                SDA( (dat & 0x80) >> 7 );
                delay_us(1);
                SCL(1);
                delay_us(5);
                SCL(0);
                delay_us(5);
                dat<<=1;
        }
}

/******************************************************************
 * 函 数 名 称：Read_Byte
 * 函 数 说 明：IIC读时序
 * 函 数 形 参：无
 * 函 数 返 回：读到的数据
 * 作       者：LC
 * 备       注：无
******************************************************************/
unsigned char Read_Byte(void)
{
    unsigned char i,receive=0;
    SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
    {
        SCL(0);
        delay_us(5);
        SCL(1);
        delay_us(5);
        receive<<=1;
        if( SDA_GET() )
        {
            receive|=1;
        }
        delay_us(5);
    }

    SCL(0);

    return receive;
}


/******************************************************************
 * 函 数 名 IICwriteBytes
 * 函 数 说 明：IIC连续写入数据
 * 函 数 形 参：addr器件地址 regaddr寄存器地址 num要写入的长度 regdata写入的数据地址
 * 函 数 返 回：0=读取成功   其他=读取失败
 * 作       者：LC
 * 备       注：无
******************************************************************/
uint8_t IICwriteBytes(uint8_t addr,uint8_t regaddr,uint8_t num,uint8_t *regdata)
{
    uint16_t i = 0;
    IIC_Start();
    Send_Byte((addr<<1)|0);
    if( I2C_WaitAck() == 1 ) {IIC_Stop();return 1;}
    Send_Byte(regaddr);
    if( I2C_WaitAck() == 1 ) {IIC_Stop();return 2;}

    for(i=0;i<num;i++)
    {
        Send_Byte(regdata[i]);
        if( I2C_WaitAck() == 1 ) {IIC_Stop();return (3+i);}
    }

    IIC_Stop();
    return 0;
}



/******************************************************************
 * 函 数 名 IICreadBytes
 * 函 数 说 明：IIC连续读取数据
 * 函 数 形 参：addr器件地址 regaddr寄存器地址 num要读取的长度 Read读取到的数据要存储的地址
 * 函 数 返 回：0=读取成功   其他=读取失败
 * 作       者：LC
 * 备       注：无
******************************************************************/
uint8_t IICreadBytes(uint8_t addr, uint8_t regaddr,uint8_t num,uint8_t* Read)
{
        uint8_t i;
        IIC_Start();
        Send_Byte((addr<<1)|0);
        if( I2C_WaitAck() == 1 ) {IIC_Stop();return 1;}
        Send_Byte(regaddr);
        if( I2C_WaitAck() == 1 ) {IIC_Stop();return 2;}

        IIC_Start();
        Send_Byte((addr<<1)|1);
        if( I2C_WaitAck() == 1 ) {IIC_Stop();return 3;}

        for(i=0;i<(num-1);i++){
                Read[i]=Read_Byte();
                IIC_Send_Ack(0);
        }
        Read[i]=Read_Byte();
        IIC_Send_Ack(1);
        IIC_Stop();
        return 0;
}































