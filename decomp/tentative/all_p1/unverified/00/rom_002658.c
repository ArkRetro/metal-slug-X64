/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x002658.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2658(void)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  ushort uVar4;

  if ((DAT_0010fdaf != '\x01') && (DAT_0010fdaf != '\x02')) {
    DAT_0010fdb4 = 0;
    return;
  }
  uVar2 = 0;
  uVar3 = 3;
  do {
    if (((&DAT_0010fdb6)[(short)uVar3] == '\0') || ((&DAT_0010fdb6)[(short)uVar3] == '\x02')) {
      uVar2 = uVar2 | 1 << (uVar3 & 0x1f);
    }
    uVar4 = (short)uVar3 - 1;
    uVar3 = (uint)uVar4;
  } while (uVar4 != 0xffff);
  bVar1 = ~DAT_0010f004 & DAT_0010f003 & (byte)uVar2 & DAT_0010fdb4;
  if (DAT_0010fd82 == '\0') {
    candidate_2624();
    bVar1 = candidate_2624();
  }
  if ((DAT_0010fd82 != '\0') || (DAT_0010fdaf == '\x02')) {
    DAT_0010f004 = bVar1 | DAT_0010f004;
  }
  DAT_0010fdb4 = bVar1;
  if (bVar1 != 0) {
    DAT_0010fdaf = '\x02';
  }
  return;
}
