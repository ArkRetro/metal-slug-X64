/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0026fa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined1 candidate_26fa(void)

{
  byte bVar1;
  byte bVar2;
  undefined1 uVar3;
  uint uVar4;
  ushort uVar5;

  uVar3 = DAT_0010fdb4;
  uVar4 = 3;
  do {
    bVar1 = '\x01' << (uVar4 & 7);
    bVar2 = DAT_0010f004 & bVar1;
    DAT_0010f004 = DAT_0010f004 & ~bVar1;
    if (bVar2 != 0) {
      (&DAT_0010fdb6)[(short)uVar4] = 1;
    }
    uVar5 = (short)uVar4 - 1;
    uVar4 = (uint)uVar5;
  } while (uVar5 != 0xffff);
  DAT_00100088 = DAT_0010fdaf;
  return uVar3;
}
