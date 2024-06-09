#include "filter.h"


/************************ 移动平均 *****************************/
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




float fc = 40.0f;     //截止频率
float Ts = 0.001f;    //采样周期
float pi = 3.14159f; //π
float alpha = 0;     //滤波系数


/************************ 一阶低通滤波 *****************************/
void low_pass_filter_init(void)
{
  float b = 2.0 * pi * fc * Ts;
  alpha = b / (b + 1);
}

float low_pass_filter(float value)
{
  static float out_last = 0; //上一次滤波值
  float out;

  /***************** 如果第一次进入，则给 out_last 赋值 ******************/
  static char fisrt_flag = 1;
  if (fisrt_flag == 1)
  {
    fisrt_flag = 0;
    out_last = value;
  }

  /*************************** 一阶滤波 *********************************/
  out = out_last + alpha * (value - out_last);
  out_last = out;

  return out;
}



