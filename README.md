# SIMD instructions
## Overview

The goal of this project is implementing SIMD instructions for optimizing calculation speed of our programm.

Project is split in two parts: Mandlebrot fractal and alpha blending. Each one shows, how SIMD instructions can maximize perfomance of our programm.

# A little bit of theory about SIMD instructions

SIMD (Single Instruction, Multiple Data) is a technique that allows a single instruction to be executed on multiple pieces of data at the same time, thus improving performance. SSE (Advanced Vector Extensions) is a specific implementation of SIMD that allows for 128-bit vectors to be processed at once.

While compiler optimization flags such as -Ofast and -O3 can improve program performance, they may not always be able to take advantage of specific hardware features like SIMD. In contrast, using SIMD SSE instructions allows for more precise control over how data is processed, resulting in potentially greater performance gains.

Additionally, compiler flags can sometimes result in unintended consequences, such as changes to program behavior or increased memory usage. By using explicit SIMD instructions, we can ensure that our program behaves exactly as intended.

For example, SIMD instructions include Intel SSE and AVX instructions, in this project we will use both of them.


# Chapter I: Mandelbrot fractal
## Mathematical description

Each dot of this set is calculated independently according to the algorithm:

Let's take a sequence on a complex plain wich looks like this:

$Z_{n + 1} = Z_n ^ 2 + C_0$

Where $C_0$ is the point on the complex plane.

The color of the point depends on how much iterations were made before the distance from $Z_n$ to $C_0$ got bigger than 2.

Here is the example of Mandelbrot fracral, drawn as a result of my programm. 

 </br>
 </br>
<img src="https://github.com/aleksplast/SIMD/blob/main/pictures/2023-04-07%20(2).png" width="500px"/>
 </br>
 </br>
 
 ## First steps
 
 Lets try to calculate each dot independently. Our code goes like this: 
 ~~~C++
for (int i = 0; i < SIZEY; i++)
{
        float y0 = (float) i * YSHIFT - 2;

        for (int j = 0; j < SIZEX; j++)
        {
            float x0 = (float) j * XSHIFT - 2;
            float x = x0;
            float y = y0;
            BYTE n = 0;

                for (; n < NMAX; n++)
                {
                    float x2 = x * x;
                    float y2 = y * y;

                    if (x2 + y2 > RMAX)
                    {
                        break;
                    }

                    y = x * y + x * y + y0;
                    x = x2 - y2 + x0;
                }
        }
}
~~~

Lets try to optimize our programm with optimization flags, like -O3, -Ofast. The output FPS is shown in the table below. We will take standart speed as a speed of our programm without AVX, but with -Ofast flag.

| Version      | Compilation flags | FPS           | Speed growth  |
| ------      | :---------------: | :------------: | :----------: |
| No AVX      | none              | 9.6            |   0.56        |
| No AVX      | -О3               | 17             |   1        |
| No AVX      | -Оfast            | 17           |   1          |

From this table we can see, that optimization flags make difference, but we will try to beat them in optimization battle, for this we will need help of AVX commands.

## Implemeting AVX commands

In our programm with them goes like this.


~~~C++
__m256 x2 = _mm256_mul_ps(x, x);
__m256 y2 = _mm256_mul_ps(y, y);

__m256 dist = _mm256_add_ps(x2, y2);
__m256 Rcmp = _mm256_cmp_ps(rad, dist, _CMP_GT_OQ);
mask = _mm256_and_ps(Rcmp, mask);
colorscnr = _mm256_add_ps(colorscnr, mask);

if (IsZero(mask))
{
    for (int i = 0; i < 8; i++)
    {
        colors[i] = (BYTE) colorscnr[i];
    }
    return 0;
}

y = _mm256_mul_ps(_mm256_add_ps(_mm256_mul_ps(_mm256_mul_ps(x, y), twos), y0), mask);
x = _mm256_mul_ps(_mm256_add_ps(_mm256_sub_ps(x2, y2), x0), mask);

~~~

The output FPS with AVX commands is shown in the table below. Just as in the first step we will try various optimization flag in addition to AVX commands.

| Version      | Compilation flags | FPS           | Speed growth  |
| ------      | :---------------: | :------------: | :----------: |
| AVX         | none              | 21             |   1.56      |
| AVX         | -О3               | 50             |   2.94       |
| AVX         | -Ofast            | 50             |   2.94     |

In addition, we can switch off graphical part of out programm and the resulting FPS will be around 80. Speed growth in this example will be 4.7 . 

## Conclusion

AVX commands is doing their job in accelerating our code, with them it goes around 3 times faster, then without them. 

Graphical part is still a big issue, without it programm runs even faster. Optimization flags also doing their impact, but not as big as AVX.

# Chapter II: Alpha Blending
# Overview

In this chapter we will use SIMD instructions for optimizing alpha blending algorithm.

# How Alpha Blending works

Lets take two pictures in RGBA format, where RGB stands for Red, Green and Blue colors and A stands for transperensy of a pixel. We will try to impose one picture on top of the other, where the resulting color is calculated according to this formula:

$ColorNew_{red} = ColorBack_{red} * \alpha + ColorFront_{red} * (\alpha - 1)$

For example, we will use two pictures: ping pong table and the very cute Aschat cat. The result must be as follow:

 </br>
 </br>
<img src="https://github.com/aleksplast/SIMD/blob/main/pictures/2023-04-07%20(3).png" width="500px"/>
 </br>
 </br>


# First steps

As in chapter I, we will try to calculate each pixel separately in the first place. Our code then goes like this:

~~~C++
for (int i = 0; i < SIZEY; i++)
{
    for (int j = 0; j < SIZEX; j++)
    {
        mem[INDEX].rgbBlue = (front[INDEX].rgbBlue * front[INDEX].rgbReserved + back[INDEX].rgbBlue * (255 - front[INDEX].rgbReserved)) >> 8;
        mem[INDEX].rgbRed = (front[INDEX].rgbRed * front[INDEX].rgbReserved + back[INDEX].rgbRed * (255 -  front[INDEX].rgbReserved)) >> 8;
        mem[INDEX].rgbGreen = (front[INDEX].rgbGreen * front[INDEX].rgbReserved + back[INDEX].rgbGreen * (255 - front[INDEX].rgbReserved)) >> 8;
    }
}
~~~

Again, lets try various optimization flags, FPS for each case is shown in the table below.

| Version      | Compilation flags | FPS           | Speed growth  |
| ------      | :---------------: | :------------: | :----------: |
| No SSE      | none              | 72            |   0.57        |
| No SSE      | -О3               | 126             |   1        |
| No SSE      | -Оfast            | 126          |   1          |

But this is not the best what we can can, lets implement SIMD instructions.

## Implementing SIMD instructions

For this task algorithm is not as easy as for Mandelbrot one. We will calculate 4 pixels at time, but for arithmetic operations will split them in two parts: first two and second two. After the arithmetic done, we will return to one array and end one iteration of the loop. Final code goes like this:

~~~C++
for (int y = 0; y < SIZEY; y++)
 {
    for (int x = 0; x < SIZEX; x += 4)
    {
        __m128i frontL  = _mm_loadu_si128((__m128i*)(front + y * SIZEX+ x));
        __m128i backL   = _mm_loadu_si128((__m128i*)(back + y * SIZEX + x));

        __m128i frontH  = (__m128i) (_mm_movehl_ps((__m128)zeros, (__m128)frontL));
        __m128i backH   = (__m128i) (_mm_movehl_ps((__m128)zeros, (__m128)backL));

        frontL  = _mm_cvtepi8_epi16(frontL);
        backL   = _mm_cvtepi8_epi16(backL);
        frontH  = _mm_cvtepi8_epi16(frontH);
        backH   = _mm_cvtepi8_epi16(backH);

        __m128i mask            = _mm_set_epi8(0x80, 14, 0x80, 14, 0x80, 14, 0x80, 14, 0x80, 6, 0x80, 6, 0x80, 6, 0x80, 6);
        __m128i transparencyL   = _mm_shuffle_epi8(frontL, mask);
        __m128i transparencyH   = _mm_shuffle_epi8(frontH, mask);

        frontL  = _mm_mullo_epi16(frontL, transparencyL);
        backL   = _mm_mullo_epi16(backL, _mm_sub_epi16(_mm_set1_epi16(255), transparencyL));
        frontH  = _mm_mullo_epi16(frontH, transparencyH);
        backH   = _mm_mullo_epi16(backH, _mm_sub_epi16(_mm_set1_epi16(255), transparencyH));

        __m128i sumL = _mm_add_epi16(frontL, backL);
        __m128i sumH = _mm_add_epi16(frontH, backH);

        mask = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 15, 13, 11, 9, 7, 5, 3, 1);
        sumL = _mm_shuffle_epi8(sumL, mask);
        sumH = _mm_shuffle_epi8(sumH, mask);

        __m128i res = (__m128i) (_mm_movelh_ps((__m128)sumL, (__m128)sumH));

        _mm_storeu_si128((__m128i*)(mem + y * SIZEX + x), res);
    }
}
~~~

As always, will try our code with various optimization flags.

Results:

| Version      | Compilation flags | FPS           | Speed growth  |
| ------      | :---------------: | :------------: | :----------: |
| SSE         | none              | 76             |   0.60      |
| SSE         | -О3               | 132             |   1.05       |
| SSE         | -Ofast            | 138            |   1.09     |

Speed growth is not as high as we want , but what if we switch off graphical part and concetrate only on calculation algorithm?

| Version      | Compilation flags | FPS           | Speed growth  |
| ------      | :---------------: | :------------: | :----------: |
| No SSE      | none              | 153            |   0.13        |
| No SSE      | -О3               | 1200             |   0.95        |
| No SSE      | -Оfast            | 1260          |   1          |
| SSE         | none              | 173             |   0.14      |
| SSE         | -О3               | 3200             |   2.53       |
| SSE         | -Ofast            | 3200           |   2.53     |

As we can see, programm runs faster, so SIMD instructions is doing their job.

## Conclusion

SIMD instructions is useful for alpha blending, just as for drawing Mandelbrot fractal. 
