#pragma once

class Font
{
    friend class OpenGL;
public:
    Font (void)
    {
        for (int i = 0; i < 32; i++)
        {
            m_fontWidth[i] = 0;
        }
        m_fontWidth[32] = 5;m_fontWidth[33] = 5;m_fontWidth[34] = 7;
        m_fontWidth[35] = 10;m_fontWidth[36] = 9;m_fontWidth[37] = 11;m_fontWidth[38] = 10;m_fontWidth[39] = 5;
        m_fontWidth[40] = 7;m_fontWidth[41] = 7;m_fontWidth[42] = 9;m_fontWidth[43] = 10;m_fontWidth[44] = 5;
        m_fontWidth[45] = 7;m_fontWidth[46] = 5;m_fontWidth[47] = 9;m_fontWidth[48] = 9;m_fontWidth[49] = 7;
        m_fontWidth[50] = 9;m_fontWidth[51] = 9;m_fontWidth[52] = 8;m_fontWidth[53] = 9;m_fontWidth[54] = 9;
        m_fontWidth[55] = 9;m_fontWidth[56] = 9;m_fontWidth[57] = 9;m_fontWidth[58] = 6;m_fontWidth[59] = 6;
        m_fontWidth[60] = 11;m_fontWidth[61] = 11;m_fontWidth[62] = 11;m_fontWidth[63] = 8;m_fontWidth[64] = 11;
        m_fontWidth[65] = 9;m_fontWidth[66] = 8;m_fontWidth[67] = 8;m_fontWidth[68] = 9;m_fontWidth[69] = 8;
        m_fontWidth[70] = 7;m_fontWidth[71] = 9;m_fontWidth[72] = 10;m_fontWidth[73] = 6;m_fontWidth[74] = 7;
        m_fontWidth[75] = 9;m_fontWidth[76] = 8;m_fontWidth[77] = 10;m_fontWidth[78] = 9;m_fontWidth[79] = 10;
        m_fontWidth[80] = 8;m_fontWidth[81] = 9;m_fontWidth[82] = 9;m_fontWidth[83] = 8;m_fontWidth[84] = 10;
        m_fontWidth[85] = 9;m_fontWidth[86] = 9;m_fontWidth[87] = 11;m_fontWidth[88] = 8;m_fontWidth[89] = 8;
        m_fontWidth[90] = 8;m_fontWidth[91] = 7;m_fontWidth[92] = 9;m_fontWidth[93] = 7;m_fontWidth[94] = 12;
        m_fontWidth[95] = 9;m_fontWidth[96] = 8;m_fontWidth[97] = 8;m_fontWidth[98] = 8;m_fontWidth[99] = 7;
        m_fontWidth[100] = 8;m_fontWidth[101] = 8;m_fontWidth[102] = 6;m_fontWidth[103] = 9;m_fontWidth[104] = 9;
        m_fontWidth[105] = 4;m_fontWidth[106] = 7;m_fontWidth[107] = 8;m_fontWidth[108] = 4;m_fontWidth[109] = 10;
        m_fontWidth[110] = 8;m_fontWidth[111] = 8;m_fontWidth[112] = 8;m_fontWidth[113] = 8;m_fontWidth[114] = 7;
        m_fontWidth[115] = 7;m_fontWidth[116] = 7;m_fontWidth[117] = 8;m_fontWidth[118] = 8;m_fontWidth[119] = 10;
        m_fontWidth[120] = 8;m_fontWidth[121] = 8;m_fontWidth[122] = 8;m_fontWidth[123] = 8;m_fontWidth[124] = 6;
        m_fontWidth[125] = 8;m_fontWidth[126] = 10;m_fontWidth[127] = 14;m_fontWidth[128] = 9;m_fontWidth[129] = 14;
        m_fontWidth[130] = 5;m_fontWidth[131] = 9;m_fontWidth[132] = 7;m_fontWidth[133] = 14;m_fontWidth[134] = 9;
        m_fontWidth[135] = 9;m_fontWidth[136] = 8;m_fontWidth[137] = 21;m_fontWidth[138] = 9;m_fontWidth[139] = 7;
        m_fontWidth[140] = 14;m_fontWidth[141] = 14;m_fontWidth[142] = 8;m_fontWidth[143] = 14;m_fontWidth[144] = 14;
        m_fontWidth[145] = 5;m_fontWidth[146] = 5;m_fontWidth[147] = 7;m_fontWidth[148] = 7;m_fontWidth[149] = 9;
        m_fontWidth[150] = 9;m_fontWidth[151] = 13;m_fontWidth[152] = 8;m_fontWidth[153] = 12;m_fontWidth[154] = 8;
        m_fontWidth[155] = 7;m_fontWidth[156] = 14;m_fontWidth[157] = 14;m_fontWidth[158] = 8;m_fontWidth[159] = 10;
        m_fontWidth[160] = 5;m_fontWidth[161] = 6;m_fontWidth[162] = 9;m_fontWidth[163] = 9;m_fontWidth[164] = 9;
        m_fontWidth[165] = 9;m_fontWidth[166] = 8;m_fontWidth[167] = 9;m_fontWidth[168] = 8;m_fontWidth[169] = 13;
        m_fontWidth[170] = 8;m_fontWidth[171] = 10;m_fontWidth[172] = 12;m_fontWidth[173] = 7;m_fontWidth[174] = 13;
        m_fontWidth[175] = 9;m_fontWidth[176] = 8;m_fontWidth[177] = 12;m_fontWidth[178] = 8;m_fontWidth[179] = 8;
        m_fontWidth[180] = 8;m_fontWidth[181] = 9;m_fontWidth[182] = 9;m_fontWidth[183] = 6;m_fontWidth[184] = 8;
        m_fontWidth[185] = 8;m_fontWidth[186] = 8;m_fontWidth[187] = 10;m_fontWidth[188] = 16;m_fontWidth[189] = 16;
        m_fontWidth[190] = 16;m_fontWidth[191] = 8;m_fontWidth[192] = 11;m_fontWidth[193] = 11;m_fontWidth[194] = 11;
        m_fontWidth[195] = 11;m_fontWidth[196] = 11;m_fontWidth[197] = 11;m_fontWidth[198] = 14;m_fontWidth[199] = 9;
        m_fontWidth[200] = 8;m_fontWidth[201] = 8;m_fontWidth[202] = 8;m_fontWidth[203] = 8;m_fontWidth[204] = 6;
        m_fontWidth[205] = 6;m_fontWidth[206] = 6;m_fontWidth[207] = 6;m_fontWidth[208] = 11;m_fontWidth[209] = 9;
        m_fontWidth[210] = 10;m_fontWidth[211] = 10;m_fontWidth[212] = 10;m_fontWidth[213] = 10;m_fontWidth[214] = 10;
        m_fontWidth[215] = 12;m_fontWidth[216] = 10;m_fontWidth[217] = 9;m_fontWidth[218] = 9;m_fontWidth[219] = 9;
        m_fontWidth[220] = 9;m_fontWidth[221] = 10;m_fontWidth[222] = 10;m_fontWidth[223] = 9;m_fontWidth[224] = 9;
        m_fontWidth[225] = 9;m_fontWidth[226] = 9;m_fontWidth[227] = 9;m_fontWidth[228] = 9;m_fontWidth[229] = 9;
        m_fontWidth[230] = 13;m_fontWidth[231] = 7;m_fontWidth[232] = 9;m_fontWidth[233] = 9;m_fontWidth[234] = 9;
        m_fontWidth[235] = 9;m_fontWidth[236] = 6;m_fontWidth[237] = 6;m_fontWidth[238] = 4;m_fontWidth[239] = 4;
        m_fontWidth[240] = 9;m_fontWidth[241] = 9;m_fontWidth[242] = 9;m_fontWidth[243] = 9;m_fontWidth[244] = 9;
        m_fontWidth[245] = 9;m_fontWidth[246] = 9;m_fontWidth[247] = 12;m_fontWidth[248] = 9;m_fontWidth[249] = 9;
        m_fontWidth[250] = 9;m_fontWidth[251] = 9;m_fontWidth[252] = 9;m_fontWidth[253] = 9;m_fontWidth[254] = 9;
        m_fontWidth[255] = 9;
    }

    inline unsigned GetSize (char* string, int limit = -1);
    inline unsigned short GetCharSize (char letter);
    inline unsigned GetLastSize (char* string, int limit);

private:
    unsigned char m_fontWidth[256];
};

inline unsigned Font::GetSize (char* string, int limit)
{
    if (!string)
    {
        return 0;
    }
    unsigned lenght = 0;
    unsigned pointer = 0;
    int i = 0;
    while (*(string+pointer) != '\0')
    {
        if (limit != -1 && i >= limit)
        {
            break;
        }
        i++;
        lenght += m_fontWidth[(unsigned char)*(string+pointer++)];
    }
    return lenght;
}

inline unsigned short Font::GetCharSize (char letter)
{
    return m_fontWidth[(unsigned char)letter];
}

inline unsigned Font::GetLastSize (char* string, int limit)
{
    if (!string)
    {
        return 0;
    }
    unsigned lenght = 0;
    unsigned pointer = 0;
    int i = 0;
    while (*(string+pointer) != '\0')
    {
        lenght += m_fontWidth[(unsigned char)*(string+pointer++)];
        if (lenght >= limit)
        {
            break;
        }
        i++;
    }
    return i;
}