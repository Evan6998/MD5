# MD5算法说明文档
## 简介
> MD5消息摘要算法（英语：MD5 Message-Digest Algorithm），一种被广泛使用的密码散列函数，可以产生出一个128位（16字节）的散列值（hash value），用于确保信息传输完整一致。

## 算法流程
- Step 1. Append Padding Bits  
    > The message is "padded" (extended) so that its length (in bits) is congruent to 448, modulo 512.         

    首先将 message padding成模512余448的长度。即差64bits就是512的整数倍。差的64bits将在下一个step中扩展出来。
    > a single "1" bit is appended to the
   message, and then "0" bits are appended so that the length in bits of
   the padded message becomes congruent to 448, modulo 512.

   扩展后，在扩展的这些位中的第一个位填充1， 其余填充0.

- Step 2. Append Length
    > A 64-bit representation of b (the length of the message before the padding bits were added) is appended to the result of the previous step.These bits are appended as two 32-bit words and appended low-order word first in accordance with the previous conventions.

    将一个64位的代表message初始长度的值append到前面提到的64的空缺中，按照小端的方式填充即可。
- Step 3. Initialize MD Buffer
    在这个step中我们需要维护四个寄存器的值用来计算信息摘要。每个寄存器均为32位,并具有如下初始值：
    ```c
    uint32_t A = 0x67452301;
    uint32_t B = 0xefcdab89;
    uint32_t C = 0x98badcfe;
    uint32_t D = 0x10325476;
    ```
- Step 4. Process Message in 16-Word Blocks  
在前面我们已经知道原本的message已经被append成了位数为512的倍数的长度。  
所以在这一步中，我们会将message分块，每块16个words，即512个bits。  
在每个块（block）中，做64次的main loop。

    ````c
    //Process the message in successive 512-bit chunks:
    for each 512-bit chunk of message
    break chunk into sixteen 32-bit little-endian words w[i], 0 ≤ i ≤ 15

    //Initialize hash value for this chunk:
    var int a := A
    var int b := B
    var int c := C
    var int d := D

    //Main loop:
    for i from 0 to 63
        if 0 ≤ i ≤ 15 then
            f := (b and c) or ((not b) and d)
            g := i
        else if 16 ≤ i ≤ 31
            f := (d and b) or ((not d) and c)
            g := (5×i + 1) mod 16
        else if 32 ≤ i ≤ 47
            f := b xor c xor d
            g := (3×i + 5) mod 16
        else if 48 ≤ i ≤ 63
            f := c xor (b or (not d))
            g := (7×i) mod 16
 
        temp := d
        d := c
        c := b
        b := leftrotate((a + f + k[i] + w[g]),r[i]) + b
        a := temp
    Next i
    //Add this chunk's hash to result so far:
    A := A + a
    B := B + b 
    C := C + c
    D := D + d
    End ForEach
    var int digest := A append B append C append D //(expressed as little-endian)
    ````
    伪代码如上所示，来源[Wiki](https://zh.wikipedia.org/zh-cn/MD5#.E4.BC.AA.E4.BB.A3.E7.A0.81)  
    在每个块的循环后将前面的四个寄存器的值连起来即可得到digest。
