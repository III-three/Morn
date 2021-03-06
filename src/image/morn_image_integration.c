/*
Copyright (C) 2019  JingWeiZhangHuai
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "morn_image.h"

void ImageIntegration(MImage *src,MTable **dst)
{
    int i,j,cn;
    int height,width;
    int sum;
    
    mException(INVALID_IMAGE(src)||INVALID_POINTER(dst),EXIT,"invlid input");
    
    height = src->height;
    width = src->width;
    for(cn=0;cn<src->channel;cn++)
    {
        mException(INVALID_POINTER(dst[cn]),EXIT,"invalid input");
        mTableRedefine(dst[cn],height+1,width+1,S32,dst[cn]->data);
        memset(dst[cn]->dataS32[0],0,(width+1)*sizeof(float));
        
        for(j=ImageY1(src);j<ImageY2(src);j++)
        {
            sum = 0;
            for(i=ImageX1(src,j);i<ImageX2(src,j);i++)
            {
                sum = sum + src->data[cn][j][i];
                dst[cn]->dataS32[j+1][i+1] = dst[cn]->dataS32[j][i+1]+sum;
            }
        }
    }
}
    
void ImageIntegration2(MImage *src,MTable **dst)
{
    int i,j,cn;
    int height,width;
    int sum;
    
    mException(INVALID_IMAGE(src)||INVALID_POINTER(dst),EXIT,"invlid input");
    
    
    
    height = src->height;
    width = src->width;
    for(cn=0;cn<src->channel;cn++)
    {
        mException(INVALID_POINTER(dst[cn]),EXIT,"invalid input");
        mTableRedefine(dst[cn],height+1,width+1,S32,dst[cn]->data);
        
        memset(dst[cn]->dataS32[0],0,(width+1)*sizeof(float));
        for(j=ImageY1(src);j<ImageY2(src);j++)
        {
            sum = 0;
            for(i=ImageX1(src,j);i<ImageX2(src,j);i++)
            {
                sum = sum + (src->data[cn][j][i])*(src->data[cn][j][i]);
                dst[cn]->dataS32[j+1][i+1] = dst[cn]->dataS32[j][i+1]+sum;
            }
        }
    }
}

void mImageIntegration(MImage *src,MTable **sum,MTable **sqsum)
{
    int i,j,cn;
    int height,width;
    int data_sum;
    int data_sqsum;
    
    if(INVALID_POINTER(sqsum))
    {
        ImageIntegration(src,sum);
        return;
    }
    
    if(INVALID_POINTER(sum))
    {
        ImageIntegration2(src,sqsum);
        return;
    }
    
    mException(INVALID_IMAGE(src),EXIT,"invalid input");
    
    height = src->height;
    width = src->width;
    for(cn=0;cn<src->channel;cn++)
    {
        mException(INVALID_POINTER(sum[cn])||INVALID_POINTER(sqsum[cn]),EXIT,"invalid input");
        mTableRedefine(sum[cn]  ,height+1,width+1,S32,  sum[cn]->data);
        mTableRedefine(sqsum[cn],height+1,width+1,S32,sqsum[cn]->data);
        
        memset(sum[cn]->data[0],0,(width+1)*sizeof(float));
        memset(sqsum[cn]->data[0],0,(width+1)*sizeof(float));
        for(j=ImageY1(src);j<ImageY2(src);j++)
        {
            data_sum = 0;
            data_sqsum = 0;
            for(i=ImageX1(src,j);i<ImageX2(src,j);i++)
            {
                data_sum = data_sum + (src->data[cn][j][i]);
                data_sqsum = data_sqsum + (src->data[cn][j][i])*(src->data[cn][j][i]);
                sum[cn]->dataS32[j+1][i+1] = sum[cn]->dataS32[j][i+1]+data_sum;
                sqsum[cn]->dataS32[j+1][i+1] = sqsum[cn]->dataS32[j][i+1]+data_sqsum;
            }
        }
    }
}

void mImageColSum(MImage *src,int **sum,int **sum2)
{
    int i,j,cn;
    int height,width;
    unsigned char **data;
    int *s,*s2;
    
    mException(INVALID_IMAGE(src),EXIT,"invalid input");
    
    height = src->height;
    width = src->width;
    
    if((sum!=NULL)&&(sum2!=NULL))
    {
        for(cn=0;cn<src->channel;cn++)
        {
            s = sum[cn];
            s2= sum2[cn];
            data = src->data[cn];
            memset(s ,0,width*sizeof(int));
            memset(s2,0,width*sizeof(int));
    
            for(j=0;j<height;j++)
                for(i=0;i<width;i++)
                {
                    s[i] = s[i] + data[j][i];
                    s2[i]= s2[i]+ data[j][i]*data[j][i];
                }
        }
    }
    else if((sum!=NULL)&&(sum2==NULL))
    {
        for(cn=0;cn<src->channel;cn++)
        {
            s = sum[cn];
            data = src->data[cn];
            memset(s ,0,width*sizeof(int));
    
            for(j=0;j<height;j++)
                for(i=0;i<width;i++)
                    s[i] = s[i] + data[j][i];
        }
    }
    else if((sum==NULL)&&(sum2!=NULL))
    {
        for(cn=0;cn<src->channel;cn++)
        {
            s2= sum2[cn];
            data = src->data[cn];
            memset(s2,0,width*sizeof(int));
    
            for(j=0;j<height;j++)
                for(i=0;i<width;i++)
                    s2[i]= s2[i]+ data[j][i]*data[j][i];
        }
    }
    else
        mException(1,EXIT,"invalid input");
}

void mImageRowSum(MImage *src,int **sum,int **sum2)
{
    int i,j,cn;
    int height,width;
    unsigned char *data;
    int s,s2;
    
    mException(INVALID_IMAGE(src),EXIT,"invalid input");
    
    height = src->height;
    width = src->width;
    
    if((sum!=NULL)&&(sum2!=NULL))
    {
        for(cn=0;cn<src->channel;cn++)
            for(j=0;j<height;j++)
            {
                s = 0;
                s2 = 0;
                data = src->data[cn][j];
                for(i=0;i<width;i++)
                {
                    s = s + data[i];
                    s2= s2+ data[i]*data[i];
                }
                sum[cn][j] = s;
                sum2[cn][j] = s2;
            }
    }
    else if((sum!=NULL)&&(sum2==NULL))
    {
        for(cn=0;cn<src->channel;cn++)
            for(j=0;j<height;j++)
            {
                s = 0;
                data = src->data[cn][j];
                for(i=0;i<width;i++)
                    s = s + data[i];
                sum[cn][j] = s;
            }
    }
    else if((sum!=NULL)&&(sum2!=NULL))
    {
        for(cn=0;cn<src->channel;cn++)
            for(j=0;j<height;j++)
            {
                s2 = 0;
                data = src->data[cn][j];
                for(i=0;i<width;i++)
                    s2= s2+ data[i]*data[i];
                sum2[cn][j] = s2;
            }
    }
    else
        mException(1,EXIT,"invalid input");
}
