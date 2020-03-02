/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			  GLCD.h
** Descriptions:		TFT (IO)
**
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2015-1-26
** Version:				  2.0
** Descriptions:		Modificada para s�lo para el controlador ILI9325
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <LPC17xx.H>
#include "GLCD.h" 
#include "AsciiLib.h"


/* La orientaci�n se modifica en GLCD.h */

/* Private variables ---------------------------------------------------------*/
static uint8_t LCD_Code;

/* Private define ------------------------------------------------------------*/
#define  ILI9320     0  /* 0x9320 */
#define  ILI9325     1  /* 0x9325 */
#define  ILI9325A    2  /* 0x9325A */
#define  ILI9325C    3  /* 0x9325C */
#define  ILI9325D    4  /* 0x9325D */

/*******************************************************************************
* Function Name  : Lcd_Configuration
* Description    : Configures LCD Control lines
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_Configuration(void)
{
	/* Configure the LCD Control pins */

    	/* DB[0.7] = P2.0...P2.7 */ 
	LPC_GPIO2->FIODIR |= 0x000000FF;               /* P2.0...P2.7   Output DB[0..7] */
	
    	/* DB[8.15]= P0.15...P0.22 */
	LPC_GPIO0->FIODIR |= 0x007F8000; 	 						/* P0.15...P0.22 Output DB[8..15]*/

	/*RS = P1.27, WR = P1.28, RD = P1.29*/
	LPC_GPIO1->FIODIR |= 0x38000000; 	 						/* P1.27...P1.29 Output */


	/*CS  = P2.8 */
	LPC_GPIO2->FIODIR |= 0x00000100; 							 /* P2.8 Output */

}

/*******************************************************************************
* Function Name  : LCD_Send
* Description    : LCDд����
* Input          : - byte: byte to be sent
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_Send (uint16_t byte) 
{
	uint32_t temp;
	temp = byte;

	LPC_GPIO2->FIOPIN =  (LPC_GPIO2->FIOPIN & (~(0x000000FF))) | (temp & 0x00FF);      						  /* Write D0..D7 */
	LPC_GPIO0->FIOPIN =  (LPC_GPIO0->FIOPIN & (~(0x007F8000))) | ((temp << 7) & 0x007F8000);        /* Write D8..D15 */
}

/*******************************************************************************
* Function Name  : LCD_Read
* Description    : LCD������
* Input          : - byte: byte to be read
* Output         : None
* Return         : ���ض�ȡ��������
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) uint16_t LCD_Read (void) 
{
	uint32_t low,high;
	
	LPC_GPIO2->FIODIR &= ~(0x000000FF);             /* P2.0...P2.7   Input DB[0..7] */
	LPC_GPIO0->FIODIR &= ~(0x007F8000); 						/* P0.15...P0.22 Input DB[8..15]*/
	low  = LPC_GPIO2->FIOPIN & 0x000000ff;          /* Read D0..D7 */
	high = LPC_GPIO0->FIOPIN & 0x007f8000;          /* Read D8..D15 */
	low |= (high >> 7);
	
	LPC_GPIO2->FIODIR |= 0x000000FF;                /* P2.0...P2.7   Output DB[0..7] */
	LPC_GPIO0->FIODIR |= 0x007F8000; 								/* P0.15...P0.22 Output DB[8..15]*/
	
	return  low;
}

/*******************************************************************************
* Function Name  : LCD_WriteIndex
* Description    : LCDд�Ĵ�����ַ
* Input          : - index: �Ĵ�����ַ
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_WriteIndex(uint16_t index)
{
	LCD_CS(0);
	LCD_RS(0);
	LCD_RD(1);
	LCD_Send( index ); 
	LCD_WR(0);
	__nop();     /* delay */
	LCD_WR(1);
	LCD_CS(1);
}

/*******************************************************************************
* Function Name  : LCD_WriteData
* Description    : LCDд�Ĵ�������
* Input          : - index: �Ĵ�������
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_WriteData(uint16_t data)
{				
	LCD_CS(0);
	LCD_RS(1);   
	LCD_Send( data );
	LCD_WR(0);
	__nop();     /* delay */
	LCD_WR(1);
	LCD_CS(1);
}

/*******************************************************************************
* Function Name  : LCD_ReadData
* Description    : ��ȡ����������
* Input          : None
* Output         : None
* Return         : ���ض�ȡ��������
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) uint16_t LCD_ReadData(void)
{ 
	uint16_t value;
	
	LCD_CS(0);
	LCD_RS(1);
	LCD_WR(1);
	LCD_RD(0);
	value = LCD_Read();
	LCD_RD(1);
	LCD_CS(1);
	
	return value;
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{ 
	/* Write 16-bit Index, then Write Reg */  
	LCD_WriteIndex(LCD_Reg);         
	/* Write 16-bit Reg */
	LCD_WriteData(LCD_RegValue);  
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
	uint16_t LCD_RAM;
	
	/* Write 16-bit Index (then Read Reg) */
	LCD_WriteIndex(LCD_Reg);
	/* Read 16-bit Reg */
	LCD_RAM = LCD_ReadData();      	
	return LCD_RAM;
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_SetCursor(uint16_t Xpos,uint16_t Ypos)
{
    #if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )
	
 	uint16_t temp = Xpos;

			 Xpos = Ypos;
			 Ypos = ( MAX_X - 1 ) - temp;  

	#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )
		
	#endif

  switch( LCD_Code )
  {
     default:		 /* 0x9320 0x9325 0x5408 0x1505 0x0505 0x7783 0x4531 0x4535 */
          LCD_WriteReg(0x0020, Xpos );     
          LCD_WriteReg(0x0021, Ypos );     
	   break;   
  }
}

/*******************************************************************************
* Function Name  : delay_ms
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void delay_ms(uint16_t ms)    
{ 
	uint16_t i,j; 
	for( i = 0; i < ms; i++ )
	{ 
		for( j = 0; j < 1141; j++ );
	}
} 

/*******************************************************************************
* Function Name  : LCD_Initializtion
* Description    : Initialize TFT Controller.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Initializtion(void)
{
	uint16_t DeviceCode;
	
	delay_ms(100);
	LCD_Configuration();
	DeviceCode = LCD_ReadReg(0x0000);		/* read LCD ID	*/	
	/*  recognition different screen�� use different Initialization*/
	if( DeviceCode == 0x9325 || DeviceCode == 0x9328 )	
	{
		LCD_Code = ILI9325;
			
		LCD_WriteReg(0xB1, 0x00A5);					/* only ILI9325C have oxB1 Register */
		DeviceCode = LCD_ReadReg(0x00B1);		/* only ILI9325C have oxB1 Register */
		if ( DeviceCode == 0x00A5)
		{
			LCD_Code = ILI9325C;
			LCD_WriteReg(0xB1, 0x0000);
		}
		else
		{
			LCD_Code = ILI9325A;
		}
		LCD_WriteReg(0xE5, 0x78F0); /* set SRAM internal timing */
		LCD_WriteReg(0x01, 0x0100); /* set Driver Output Control */
		LCD_WriteReg(0x02, 0x0700); /* set 1 line inversion */
		LCD_WriteReg(0x03, 0x1030); /* set GRAM write direction and BGR=1 */
		LCD_WriteReg(0x04, 0x0000); /* Resize register */
		LCD_WriteReg(0x08, 0x0207); /* set the back porch and front porch */
		LCD_WriteReg(0x09, 0x0000); /* set non-display area refresh cycle ISC[3:0] */
		LCD_WriteReg(0x0A, 0x0000); /* FMARK function */
		LCD_WriteReg(0x0C, 0x0000); /* RGB interface setting */
		LCD_WriteReg(0x0D, 0x0000); /* Frame marker Position */
		LCD_WriteReg(0x0F, 0x0000); /* RGB interface polarity */
		/*************Power On sequence ****************/
		LCD_WriteReg(0x10, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
		LCD_WriteReg(0x11, 0x0007); /* DC1[2:0], DC0[2:0], VC[2:0] */
		LCD_WriteReg(0x12, 0x0000); /* VREG1OUT voltage */
		LCD_WriteReg(0x13, 0x0000); /* VDV[4:0] for VCOM amplitude */
		LCD_WriteReg(0x07, 0x0001);
		delay_ms(200); 
		/* Dis-charge capacitor power voltage */
		LCD_WriteReg(0x10, 0x1090); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
		LCD_WriteReg(0x11, 0x0227); /* Set DC1[2:0], DC0[2:0], VC[2:0] */
		delay_ms(50); 							/* Delay 50ms */
		LCD_WriteReg(0x12, 0x001F); 
		delay_ms(50); 							/* Delay 50ms */
		LCD_WriteReg(0x13, 0x1500); /* VDV[4:0] for VCOM amplitude */
		LCD_WriteReg(0x29, 0x0027); /* 04 VCM[5:0] for VCOMH */
		LCD_WriteReg(0x2B, 0x000D); /* Set Frame Rate */
		delay_ms(50); 							/* Delay 50ms */
		LCD_WriteReg(0x20, 0x0000); /* GRAM horizontal Address */
		LCD_WriteReg(0x21, 0x0000); /* GRAM Vertical Address */
		/* ----------- Adjust the Gamma Curve ---------- */
		LCD_WriteReg(0x30, 0x0000);
		LCD_WriteReg(0x31, 0x0707);
		LCD_WriteReg(0x32, 0x0307);
		LCD_WriteReg(0x35, 0x0200);
		LCD_WriteReg(0x36, 0x0008);
		LCD_WriteReg(0x37, 0x0004);
		LCD_WriteReg(0x38, 0x0000);
		LCD_WriteReg(0x39, 0x0707);
		LCD_WriteReg(0x3C, 0x0002);
		LCD_WriteReg(0x3D, 0x1D04);
		/* ------------------ Set GRAM area --------------- */
		LCD_WriteReg(0x50, 0x0000); /* Horizontal GRAM Start Address */
		LCD_WriteReg(0x51, 0x00EF); /* Horizontal GRAM End Address */
		LCD_WriteReg(0x52, 0x0000); /* Vertical GRAM Start Address */
		LCD_WriteReg(0x53, 0x013F); /* Vertical GRAM Start Address */
		LCD_WriteReg(0x60, 0xA700); /* Gate Scan Line */
		LCD_WriteReg(0x61, 0x0001); /* NDL,VLE, REV */
		LCD_WriteReg(0x6A, 0x0000); /* set scrolling line */
		/* -------------- Partial Display Control --------- */
		LCD_WriteReg(0x80, 0x0000);
		LCD_WriteReg(0x81, 0x0000);
		LCD_WriteReg(0x82, 0x0000);
		LCD_WriteReg(0x83, 0x0000);
		LCD_WriteReg(0x84, 0x0000);
		LCD_WriteReg(0x85, 0x0000);
		/* -------------- Panel Control ------------------- */
		LCD_WriteReg(0x90, 0x0010);
		LCD_WriteReg(0x92, 0x0600);
		LCD_WriteReg(0x07, 0x0133); /* 262K color and display ON */
	}
  						
    delay_ms(50);   /* delay 50 ms */	
}

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : ����Ļ����ָ������ɫ��������������� 0xffff
* Input          : - Color: Screen Color
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Clear(uint16_t Color)
{
	uint32_t index;
	LCD_SetCursor(0,0); 
	LCD_WriteIndex(0x0022);
	for( index = 0; index < MAX_X * MAX_Y; index++ )
	{
		LCD_WriteData(Color);
	}
}

/******************************************************************************
* Function Name  : LCD_BGR2RGB
* Description    : RRRRRGGGGGGBBBBB ��Ϊ BBBBBGGGGGGRRRRR ��ʽ
* Input          : - color: BRG ��ɫֵ  
* Output         : None
* Return         : RGB ��ɫֵ
* Attention		 : �ڲ���������
*******************************************************************************/
static uint16_t LCD_BGR2RGB(uint16_t color)
{
	uint16_t  r, g, b, rgb;
	
	b = ( color>>0 )  & 0x1f;
	g = ( color>>5 )  & 0x3f;
	r = ( color>>11 ) & 0x1f;
	
	rgb =  (b<<11) + (g<<5) + (r<<0);
	
	return( rgb );
}

/******************************************************************************
* Function Name  : LCD_GetPoint
* Description    : ��ȡָ���������ɫֵ
* Input          : - Xpos: Row Coordinate
*                  - Xpos: Line Coordinate 
* Output         : None
* Return         : Screen Color
* Attention		 : None
*******************************************************************************/
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos)
{
	uint16_t dummy;
	
	LCD_SetCursor(Xpos,Ypos);
	LCD_WriteIndex(0x0022);  
	
	switch( LCD_Code )
	{
	
		case ILI9325C:
		case ILI9325D:
             dummy = LCD_ReadData();   /* Empty read */
             dummy = LCD_ReadData(); 	
    return  dummy;	      
	    

    default:	/* 0x9320 0x9325 0x5408 0x1505 0x0505 */
             dummy = LCD_ReadData();   /* Empty read */
             dummy = LCD_ReadData(); 	
 		return  LCD_BGR2RGB( dummy );
	}
}

/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : ��ָ�����껭��
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
	if( Xpos >= MAX_X || Ypos >= MAX_Y )
	{
		return;
	}
	LCD_SetCursor(Xpos,Ypos);
	LCD_WriteReg(0x0022,point);
}

/******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : Bresenham's line algorithm
* Input          : - x1: A��������
*                  - y1: A�������� 
*				   - x2: B��������
*				   - y2: B�������� 
*				   - color: ����ɫ
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
    short dx,dy;      /* ����X Y�������ӵı���ֵ */
    short temp;       /* ��� �յ��С�Ƚ� ��������ʱ���м���� */

    if( x0 > x1 )     /* X�����������յ� �������� */
    {
	    temp = x1;
		x1 = x0;
		x0 = temp;   
    }
    if( y0 > y1 )     /* Y�����������յ� �������� */
    {
		temp = y1;
		y1 = y0;
		y0 = temp;   
    }
  
	dx = x1-x0;       /* X�᷽���ϵ����� */
	dy = y1-y0;       /* Y�᷽���ϵ����� */

    if( dx == 0 )     /* X����û������ ����ֱ�� */ 
    {
        do
        { 
            LCD_SetPoint(x0, y0, color);   /* �����ʾ �费ֱ�� */
            y0++;
        }
        while( y1 >= y0 ); 
		return; 
    }
    if( dy == 0 )     /* Y����û������ ��ˮƽֱ�� */ 
    {
        do
        {
            LCD_SetPoint(x0, y0, color);   /* �����ʾ ��ˮƽ�� */
            x0++;
        }
        while( x1 >= x0 ); 
		return;
    }
	/* ����ɭ��ķ(Bresenham)�㷨���� */
    if( dx > dy )                         /* ����X�� */
    {
	    temp = 2 * dy - dx;               /* �����¸����λ�� */         
        while( x0 != x1 )
        {
	        LCD_SetPoint(x0,y0,color);    /* ����� */ 
	        x0++;                         /* X���ϼ�1 */
	        if( temp > 0 )                /* �ж����¸����λ�� */
	        {
	            y0++;                     /* Ϊ�������ڵ㣬����x0+1,y0+1�� */ 
	            temp += 2 * dy - 2 * dx; 
	 	    }
            else         
            {
			    temp += 2 * dy;           /* �ж����¸����λ�� */  
			}       
        }
        LCD_SetPoint(x0,y0,color);
    }  
    else
    {
	    temp = 2 * dx - dy;                      /* ����Y�� */       
        while( y0 != y1 )
        {
	 	    LCD_SetPoint(x0,y0,color);     
            y0++;                 
            if( temp > 0 )           
            {
                x0++;               
                temp+=2*dy-2*dx; 
            }
            else
			{
                temp += 2 * dy;
			}
        } 
        LCD_SetPoint(x0,y0,color);
	}
} 

/******************************************************************************
* Function Name  : PutChar
* Description    : ��Lcd��������λ����ʾһ���ַ�
* Input          : - Xpos: ˮƽ���� 
*                  - Ypos: ��ֱ����  
*				   - ASCI: ��ʾ���ַ�
*				   - charColor: �ַ���ɫ   
*				   - bkColor: ������ɫ 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    GetASCIICode(buffer,ASCI);  /* ȡ��ģ���� */
    for( i=0; i<16; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<8; j++ )
        {
            if( (tmp_char >> 7 - j) & 0x01 == 0x01 )
            {
                LCD_SetPoint( Xpos + j, Ypos + i, charColor );  /* �ַ���ɫ */
            }
            else
            {
                LCD_SetPoint( Xpos + j, Ypos + i, bkColor );  /* ������ɫ */
            }
        }
    }
}

/******************************************************************************
* Function Name  : GUI_Text
* Description    : ��ָ��������ʾ�ַ���
* Input          : - Xpos: ������
*                  - Ypos: ������ 
*				   - str: �ַ���
*				   - charColor: �ַ���ɫ   
*				   - bkColor: ������ɫ 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
    uint8_t TempChar;
    do
    {
        TempChar = *str++;  
        PutChar( Xpos, Ypos, TempChar, Color, bkColor );    
        if( Xpos < MAX_X - 8 )
        {
            Xpos += 8;
        } 
        else if ( Ypos < MAX_Y - 16 )
        {
            Xpos = 0;
            Ypos += 16;
        }   
        else
        {
            Xpos = 0;
            Ypos = 0;
        }    
    }
    while ( *str != 0 );
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
