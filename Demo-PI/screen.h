// resolu��o do jogo � diferente do tamanho de janela criado, a resolu��o � definida pelo buffer e o tamanho da janela � o buffer multiplicado
// pelo escalonamento

#ifndef SCREEN_H
#define SCREEN_H

#define BUFFER_W 640
#define BUFFER_H 480
#define DISP_SCALE 1
#define DISP_W BUFFER_W * DISP_SCALE
#define DISP_H BUFFER_H * DISP_SCALE

#endif