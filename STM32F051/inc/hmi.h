#ifndef __HMI_H
#define __HMI_H
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum {
	off,
	beep_const,
	strobo,
	beeps
}e_buzzer_mode;

typedef enum {
	button_1,
	button_2,
	button_3,
	button_4,
	button_enc
}e_buttons;


/* Exported constants --------------------------------------------------------*/
extern e_buzzer_mode buzzer_mode;
extern uint8_t buzzer_beeps_counter;
extern uint16_t buzzer_beep_time;
extern uint16_t buzzer_beep_time_counter;
extern uint16_t buttons_delay[5];
extern uint16_t display_block;
extern uint8_t display[4];
extern const uint8_t charset[4][255];
extern bool b1_flag;
extern bool b2_flag;
extern bool b3_flag;
extern bool b4_flag;
typedef enum {A=0,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,Y,Z,X} alphabet;
/* Exported macro ------------------------------------------------------------*/
#define button_press_delay			500


/* Exported functions ------------------------------------------------------- */
void buzzer_speed(uint16_t speed);
void buzzer(e_buzzer_mode b_mode, uint8_t beep_count);
void button_handler(e_buttons button);
void encoder_handler(void);
void write_num_to_display(uint16_t number);
void write_txt_to_display(alphabet leftChar, alphabet middleChar, alphabet rightChar);
#endif /* __HMI_H */


