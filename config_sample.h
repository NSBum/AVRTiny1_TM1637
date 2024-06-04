#ifndef CONFIG_H_
#define CONFIG_H_

/*******************************************************
 * 
 * TM1637 configuration parameters
 * 
 *******************************************************
*/
#define TM1637_CLK_PORT			(PORTA)
#define TM1637_DIO_PORT			(PORTA)
#define	TM1637_DIO_MASK			PIN6_bm
#define	TM1637_CLK_MASK			PIN7_bm
#define TM1637_DIO_PIN			PIN6_bp
#define TM1637_CLK_PIN			PIN7_bp

#endif /* CONFIG_H_ */