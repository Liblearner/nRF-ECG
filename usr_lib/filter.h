#ifndef FILTER_H
#define FILTER_H

#define MVF_LENGTH  10
typedef float E_SAMPLE;

/*定义移动平均寄存器历史状态*/
typedef struct  _t_MAF
{
    E_SAMPLE buffer[MVF_LENGTH];
    E_SAMPLE sum;
    int index;
}t_MAF;


void moving_average_filter_init(t_MAF * pMaf);
E_SAMPLE moving_average_filter(t_MAF * pMaf,E_SAMPLE xn);
void low_pass_filter_init(void);
float low_pass_filter(float value);

#endif
