
#define	STATUS_REG_AUX	0x07
#define	OUT_ADC1_L		0x08
#define	OUT_ADC1_H		0x09
#define	OUT_ADC2_L		0x0A
#define	OUT_ADC2_H		0x0B
#define	OUT_ADC3_L		0x0C
#define	OUT_ADC3_H		0x0D
#define	INT_COUNTER_REG	0x0E
#define	WHO_AM_I		0x0F
#define	TEMP_CFG_REG	0x1F
#define	CTRL_REG1		0x20
#define	CTRL_REG2		0x21
#define	CTRL_REG3		0x22
#define	CTRL_REG4		0x23
#define	CTRL_REG5		0x24
#define	CTRL_REG6		0x25
#define	REFERENCE		0x26
#define	STATUS_REG		0x27
#define	OUT_X_L			0x28
#define	OUT_X_H			0x29
#define	OUT_Y_L			0x2A
#define	OUT_Y_H			0x2B
#define	OUT_Z_L			0x2C
#define	OUT_Z_H			0x2D
#define	FIFO_CTRL_REG	0x2E
#define	FIFO_SRC_REG	0x2F
#define	INT1_CFG		0x30
#define	INT1_SOURCE		0x31
#define	INT1_THS		0x32
#define	INT1_DURATION	0x33
#define	CLICK_CFG		0x38
#define	CLICK_SRC		0x39
#define	CLICK_THS		0x3A
#define	TIME_LIMIT		0x3B
#define	TIME_LATENCY	0x3C
#define	TIME_WINDOW		0x3D


#define	ACC_CS			SPI_CS1
#define	ACMT_NONE		0	
#define	ACMT_LIS3DH		'3'

volatile byte	AccelerometerType;
volatile signed int16	ValX, ValY, ValZ;
volatile signed int16	AValX, AValY, AValZ;
volatile signed int16	dValX, dValY, dValZ;

byte lis3dh_read(byte address) {
	byte data;
	
	ACC_CS = 0;
	spi_write(address | 0x80);	
	data = spi_read(0xFF);
	ACC_CS = 1;
	
	return data;
}


bit lis3dh_write(word address, byte data) {
	byte res;

	ACC_CS = 0;
	spi_write(address);
	spi_write(data);
	ACC_CS = 1;
	
	ACC_CS = 0;
	spi_write(address | 0x80);	
	res = spi_read(0xFF);
	ACC_CS = 1;
	
	if(res == data) return TRUE;
	//printf("<%02X>", res);
	return FALSE;
}


void init_lis3dh(void){
	byte	i;
	
	TermPeriod = 0;	
	Term = 0;
	ACC_CS = 1;
	delay_ms(100);	
	lis3dh_write(CTRL_REG5, 0x80);	// Rebooting
	delay_ms(100);	
	
	AccelerometerType = ACMT_NONE;
	printf("\r\nAcmt ");
	for(i = 0; i < 10; i++){
		if(lis3dh_write(CTRL_REG4, 0x88)) break; // BDU=1, HR=1
		//if(lis3dh_write(CTRL_REG4, 0x00)) break; // BDU=1, HR=1
		delay_ms(100);
	}	
	if(i == 10) return;
	//putchar('4');
	
	for(i = 0; i < 10; i++){
		if(lis3dh_write(CTRL_REG1, 0x17)) break; // ODR=1Hz, LPen=0, XYZ=1
		//if(lis3dh_write(CTRL_REG1, 0x00)) break; // ODR=1Hz, LPen=0, XYZ=1
		delay_ms(100);
	}	
	if(i == 10) return;
	//putchar('1');
	
	for(i = 0; i < 10; i++){
		if(lis3dh_write(CTRL_REG2, 0x00)) break;
		delay_ms(100);
	}	
	if(i == 10) return;
	//putchar('2');
	
	for(i = 0; i < 10; i++){
		if(lis3dh_write(CTRL_REG3, 0x10)) break; // DRDY=1			
		//if(lis3dh_write(CTRL_REG3, 0x00)) break; // DRDY=1			
		delay_ms(100);
	}	
	if(i == 10) return;
	//putchar('3');
	
	for(i = 0; i < 10; i++){
		if(lis3dh_write(CTRL_REG5, 0x00)) break; // 			
		delay_ms(100);
	}	
	if(i == 10) return;
	//putchar('5');
	
	for(i = 0; i < 10; i++){
		if(lis3dh_write(CTRL_REG6, 0x00)) break; // 			
		delay_ms(100);
	}	
	if(i == 10) return;
	//putchar('6');
		
	AccelerometerType = lis3dh_read(WHO_AM_I);
	if(AccelerometerType != ACMT_LIS3DH) printf(" unknown.\r\n");
	else printf(" LIS3DH OK.\r\n");	
}

#define	DENOM	16

void GetAccelerometerData(void){
	byte res;
	
	if(AccelerometerType != ACMT_LIS3DH) return;
	disable_interrupts(GLOBAL);
//	if(TermPeriod >= TERM_PERIOD){
//		TermPeriod = 0;
//		lis3dh_write(TEMP_CFG_REG, 0xC0);
//	}	
//	if(TermPeriod == 1) {
//		printf("(%02X:%02X%02X %02X%02X %02X%02X:%02X)", 
//			lis3dh_read(STATUS_REG_AUX),
//			lis3dh_read(OUT_ADC1_H), lis3dh_read(OUT_ADC1_L),
//			lis3dh_read(OUT_ADC2_H), lis3dh_read(OUT_ADC2_L),
//			lis3dh_read(OUT_ADC3_H), lis3dh_read(OUT_ADC3_L),
//			lis3dh_read(STATUS_REG_AUX),);
//		Term = lis3dh_read(OUT_ADC3_H);
//		lis3dh_write(TEMP_CFG_REG, 0x00);
//	}	
	res = lis3dh_read(STATUS_REG);
	//printf("[%02X", res);
	if(res & 0x08) {
		ValXL = lis3dh_read(OUT_X_L); ValXH = lis3dh_read(OUT_X_H); 
		ValYL = lis3dh_read(OUT_Y_L); ValYH = lis3dh_read(OUT_Y_H); 
		ValZL = lis3dh_read(OUT_Z_L); ValZH = lis3dh_read(OUT_Z_H);
		//res = lis3dh_read(STATUS_REG);
		//printf("->%02X]", res);
	}
	//else putchar(']');
	enable_interrupts(GLOBAL);	
	//ShowAccelerometerData();
}	


void ShowAccelerometerData(void){
	
	
	if(AccelerometerType != ACMT_LIS3DH) return;
	
	ValX = ((signed int16) make16(ValXH, ValXL));
	ValY = ((signed int16) make16(ValYH, ValYL));
	ValZ = ((signed int16) make16(ValZH, ValZL));
	ValX = (ValX + DENOM/2) / DENOM;
	ValY = (ValY + DENOM/2) / DENOM;
	ValZ = (ValZ + DENOM/2) / DENOM;
		
	dValX = ValX - AValX;
	dValY = ValY - AValY;
	dValZ = ValZ - AValZ;
	
	AValX = (AValX + ValX) / 2;
	AValY = (AValY + ValY) / 2;
	AValZ = (AValZ + ValZ) / 2;
	
	printf("\r\n%5ld %5ld %5ld       (%5ld,%5ld) (%5ld,%5ld) (%5ld,%5ld)", 
			ValX, ValY, ValZ, AValX, dValX, AValY, dValY, AValZ, dValZ);	

}	


