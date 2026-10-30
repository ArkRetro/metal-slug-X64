/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a748e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a748e(void)

{
  byte bVar1;
  int extraout_A0;
  int extraout_A0_00;
  int iVar2;
  int extraout_A0_01;
  int unaff_A6;

  bVar1 = *(byte *)(unaff_A6 + 0x79);
  *(byte *)(unaff_A6 + 0x79) = bVar1 | 0x80;
  if ((bVar1 & 0x80) == 0) {
    candidate_40bc();
    iVar2 = extraout_A0;
  }
  else {
    candidate_40bc();
    iVar2 = extraout_A0_00;
  }
  *(undefined1 *)(iVar2 + 0x7a) = *(undefined1 *)(unaff_A6 + 0x7a);
  *(undefined1 *)(iVar2 + 0x7b) = *(undefined1 *)(unaff_A6 + 0x7b);
  *(undefined2 *)(iVar2 + 0x6e) = *(undefined2 *)(unaff_A6 + 0x6e);
  *(undefined2 *)(iVar2 + 0x72) = *(undefined2 *)(unaff_A6 + 0x72);
  candidate_1acf2();
  *(byte *)(extraout_A0_01 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 3;
  return;
}
