# MD5 Implement
`Evan H`  
This encryption algorithm could be found in [RFC 1321](https://www.rfc-editor.org/rfc/rfc1321.txt) and [Wiki](https://zh.wikipedia.org/zh-cn/MD5).  
This algoritm could be seperate into 4 part
- Append Padding Bits
- Append Length
- Initialize MD Buffer
- Process Message in 16-Word Blocks  

respectively.  
The difficuly of this algoritm may be the **allocation of vars** and the **processing of bits**.