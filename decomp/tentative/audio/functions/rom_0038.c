/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x0038; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined1
RST7(undefined2 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,undefined2 param_5)

{
  ushort uVar1;
  undefined2 in_AF;
  undefined1 uVar2;

  uVar1 = _DAT_ram_f80b;
  uVar2 = (undefined1)(_DAT_ram_f80b >> 8);
  z80_candidate_8(CONCAT11(uVar2,4),0x2735,param_4,param_5,param_2,param_3,param_1);
  if (DAT_ram_f801 < 2) {
    DAT_ram_f801 = DAT_ram_f801 + 1;
  }
  DAT_ram_f802 = DAT_ram_f802 + '\x01';
  DAT_ram_f80a = uIo0006;
  do {
  } while (cIo0004 < '\0');
  *(undefined1 *)(uVar1 & 0xff) = uVar2;
  enableMaskableInterrupts();
  return (char)((ushort)in_AF >> 8);
}
