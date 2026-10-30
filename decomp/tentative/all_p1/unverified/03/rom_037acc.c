/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x037acc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_37acc(void)

{
  byte bVar1;
  int iVar2;
  int unaff_A6;

  *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 0x80;
  if (*(char *)(unaff_A6 + 0x97) == '\0') {
    bVar1 = *(byte *)(unaff_A6 + 0x94);
    iVar2 = *(int *)(unaff_A6 + 0xc) + 0x76;
  }
  else {
    bVar1 = *(byte *)(unaff_A6 + 0x95);
    iVar2 = *(int *)(unaff_A6 + 0xc) + 0x7e;
  }
  *(undefined4 *)(iVar2 + (short)((bVar1 & 0xf) << 2)) = 0;
  return;
}
