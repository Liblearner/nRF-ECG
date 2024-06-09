#include "filter.h"


/************************ �ƶ�ƽ�� *****************************/
void moving_average_filter_init(t_MAF * pMaf)
{
    pMaf->index = -1;
    pMaf->sum   = 0;
}
 
E_SAMPLE moving_average_filter(t_MAF * pMaf,E_SAMPLE xn)
{
    E_SAMPLE yn=0;
    int i=0;
 
    if(pMaf->index == -1)
    {
        for(i = 0; i < MVF_LENGTH; i++)
        {
            pMaf->buffer[i] = xn;
        }
        pMaf->sum   = xn*MVF_LENGTH;
        pMaf->index = 0;
    }
    else
    {
        pMaf->sum     -= pMaf->buffer[pMaf->index];
        pMaf->buffer[pMaf->index] = xn;
        pMaf->sum     += xn;
        pMaf->index++;
        if(pMaf->index>=MVF_LENGTH)
            pMaf->index = 0;
    }
    yn = pMaf->sum/MVF_LENGTH;
    return yn;
}




float fc = 40.0f;     //��ֹƵ��
float Ts = 0.001f;    //��������
float pi = 3.14159f; //��
float alpha = 0;     //�˲�ϵ��


/************************ һ�׵�ͨ�˲� *****************************/
void low_pass_filter_init(void)
{
  float b = 2.0 * pi * fc * Ts;
  alpha = b / (b + 1);
}

float low_pass_filter(float value)
{
  static float out_last = 0; //��һ���˲�ֵ
  float out;

  /***************** �����һ�ν��룬��� out_last ��ֵ ******************/
  static char fisrt_flag = 1;
  if (fisrt_flag == 1)
  {
    fisrt_flag = 0;
    out_last = value;
  }

  /*************************** һ���˲� *********************************/
  out = out_last + alpha * (value - out_last);
  out_last = out;

  return out;
}



