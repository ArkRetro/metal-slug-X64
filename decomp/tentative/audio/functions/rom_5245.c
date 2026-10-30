/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x5245; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void z80_candidate_5245(undefined2 param_1,undefined1 *param_2,undefined2 param_3)

{
  undefined2 uVar1;
  char cVar2;
  char cVar3;
  undefined1 uVar4;
  undefined1 uVar5;

  cVar2 = DAT_ram_ff13;
  uVar5 = (undefined1)((ushort)param_3 >> 8);
  uVar4 = (undefined1)param_3;
  cVar3 = DAT_ram_ff13;
  z80_candidate_57be(param_1);
  param_2[0x14] = uVar4;
  param_2[0x15] = uVar5;
  param_2[4] = cVar3;
  if (DAT_ram_ff13 != cVar2) {
    DAT_ram_ff13 = cVar2;
  }
  param_2[3] = 0;
  param_2[9] = 0;
  param_2[9] = param_2[9] | 4;
  param_2[9] = param_2[9] | 8;
  *param_2 = 0;
  param_2[1] = 0;
  param_2[10] = 0xff;
  param_2[0xb] = 0xff;
  param_2[0x16] = 0;
  param_2[0xc] = 0;
  param_2[0xd] = 0;
  param_2[0xe] = 0;
  param_2[0x10] = 0;
  param_2[0x11] = 0;
  param_2[0x12] = 0xff;
  param_2[0xf] = 3;
  param_2[0x13] = 0;
  param_2[0x22] = 6;
  uVar1 = _DAT_ram_f816;
  param_2[7] = (char)_DAT_ram_f816;
  param_2[8] = (char)((ushort)uVar1 >> 8);
  uVar1 = _DAT_ram_f818;
  param_2[5] = (char)_DAT_ram_f818;
  param_2[6] = (char)((ushort)uVar1 >> 8);
  param_2[0x17] = 0x40;
  param_2[0x18] = 0xc0;
  param_2[0x19] = 0xc0;
  param_2[0x1a] = 0x7f;
  return;
}
