/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0301ae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_301ae(void)

{
  undefined2 in_D0w;
  int iVar1;
  byte unaff_D6b;
  byte bVar2;
  byte unaff_D7b;

  iVar1 = CONCAT22(in_D0w,0xb00);
  for (bVar2 = unaff_D6b; 2 < bVar2; bVar2 = bVar2 - 1) {
  }
  for (; 2 < unaff_D7b; unaff_D7b = unaff_D7b - 1) {
    iVar1 = iVar1 + 0x10000;
    for (bVar2 = unaff_D6b; 2 < bVar2; bVar2 = bVar2 - 1) {
    }
  }
  iVar1 = CONCAT31((int3)((uint)(CONCAT31((int3)((uint)(iVar1 + 0x10000) >> 8),0xf6) + 0x200000) >>
                         8),0xf1);
  for (; 2 < unaff_D6b; unaff_D6b = unaff_D6b - 1) {
    iVar1 = iVar1 + 0x200000;
  }
  _DAT_003c0000 = CONCAT31((int3)((uint)iVar1 >> 8),0xf7);
  return;
}
