/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02bef2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

byte candidate_2bef2(void)

{
  int iVar1;
  byte bVar2;
  int unaff_A6;

  candidate_1abc8();
  iVar1 = *(int *)(unaff_A6 + 0xc);
  *(undefined1 *)(iVar1 + 0x61) = *(undefined1 *)(unaff_A6 + 0x61);
  *(undefined1 *)(unaff_A6 + 0x91) = *(undefined1 *)(iVar1 + 0x91);
  bVar2 = *(byte *)(iVar1 + 0x60) & 0xf;
  if (bVar2 != 0xf) {
    *(undefined1 *)(iVar1 + 0x60) = 0xf;
    *(byte *)(unaff_A6 + 0x60) = bVar2;
  }
  return bVar2;
}
