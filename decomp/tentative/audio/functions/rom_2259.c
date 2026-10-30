/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x2259; tentative_c. */

void z80_candidate_2259(byte *param_1,byte *param_2)

{
  byte bVar1;

  bVar1 = *param_1;
  param_2[7] = bVar1 & 0x7f;
  *param_2 = bVar1 >> 7;
  param_2[0xd] = param_1[1];
  param_2[1] = param_1[2];
  param_2[2] = param_1[3] * '\x02' + (0x7f < (byte)(param_1[3] * '\x02'));
  param_2[3] = param_1[5] * '\x02' + (0x7f < (byte)(param_1[5] * '\x02'));
  bVar1 = *(byte *)CONCAT11(7,~*(byte *)CONCAT11(6,param_1[4]));
  param_2[9] = *(byte *)CONCAT11(8,~*(byte *)CONCAT11(6,param_1[4]));
  param_2[0xb] = bVar1;
  param_2[10] = 0;
  param_2[0xc] = 0;
  return;
}
