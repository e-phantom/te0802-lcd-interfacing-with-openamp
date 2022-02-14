/*
 * lcd.h
 *
 *  Created on: Feb 8, 2022
 *      Author: hamza
 */

#ifndef SRC_LCD_H_
#define SRC_LCD_H_

#include <stdbool.h>

#define	clrBlack		0x00000000ul
#define	clrDkGray		0x00606060ul
#define	clrMedGray		0x00808080ul
#define	clrLtGray		0x00C0C0C0ul
#define	clrWhite		0x00FFFFFFul
#define	clrRed			0x00FF0000ul
#define	clrDkRed		0x00800000ul
#define	clrGreen		0x0000FF00ul
#define	clrDkGreen		0x00008000ul
#define	clrBlue			0x000000FFul
#define	clrDkBlue		0x00000080ul
#define	clrYellow		0x00FFFF00ul
#define	clrDkYellow		0x00808000ul
#define	clrCyan			0x0000FFFFul
#define	clrDkCyan		0x00008080ul
#define	clrMagenta		0x00FF00FFul
#define	clrDkMagenta	0x00800080ul
#define	clrLtBlueGray	0x00B8B8FFul
#define	clrMedBlueGray	0x007878FFul

/* Setup and status functions.
*/
void 		clearDisplay();
void 		displayOn(bool fEn);
void 		setBacklight(int val);
//HDS			displayHds();
void 		setPen(unsigned int pen);
void 		setForeground(unsigned int clr);
void 		setBackground(unsigned int clr);
void 		setTransparency(bool fTrans, unsigned int clr);
void 		setIntensity(int ity);
void 		setFont(int idFont);
unsigned int		loadImage(char * name);
void 		deleteImage(unsigned int hbmp);

/* Basic graphics drawing functions.
*/
void 		setPixel(int x, int y, unsigned int clr);
unsigned int	getPixel(int x, int y);
void 		drawLine(int x1, int y1, int x2, int y2);
void 		drawArc(int x1, int y1, int x2, int y2,	int xr1, int yr1, int xr2, int yr2);
void 		drawRectangle(bool fill, int x1, int y1, int x2, int y2);
void 		drawEllipse(bool fill, int x1, int y1, int x2, int y2);
void 		drawImage(char * name, int x, int y);
//void 		drawImage(HBMP hbmp, int x, int y);
void 		drawText(char * txt, int x, int y);

/* User interface management fuctions.
*/
void 		createButton(int id, char * name, int x, int y);
//void 		createButton(int id, char * name1, char * name2, int x, int y);
//void 		createButton(int id, HBMP hbmp1, HBMP hbmp2, int x, int y);
void 		deleteButton(int id);
void 		enableButton(int id, bool fEn);
void 		drawButton(int id, int st);
void 		isEnabled(int id);
void 		isTouched(int id);
//void 		getFinger(int num, MDFNG * fng);
void 		getButton();
void		checkTouch();

/* Extra Functions
 */
int get_response();
int send_message(char * args);
int function_call(char * args);
void setup();
void loop();
int start_app();
void exit_app();

#endif /* SRC_LCD_H_ */
