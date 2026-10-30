/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d2266.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d2266(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined1 *)(unaff_A6[3] + 0x74) = 1;
  *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) | 0x20;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_00;
  }
  *(undefined2 *)(iVar1 + 0x8e) = *(undefined2 *)((int)unaff_A6 + 0x8e);
  *unaff_A6 = 0xd22b0;
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 2;
  candidate_d3220();
  candidate_d3268();
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
    *unaff_A6 = candidate_d22d6;
  }
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
  bVar2 = false;
  if (*(short *)((int)unaff_A6 + 0x6e) < 1) {
    *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) | 0x10;
  }
  candidate_d341e();
  if (bVar2) {
    *unaff_A6 = candidate_d231a;
  }
  return;
}
