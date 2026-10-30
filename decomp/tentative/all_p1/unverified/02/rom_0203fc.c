/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0203fc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_203fc(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x8c) & 1) == 0) {
    candidate_40bc();
    iVar1 = extraout_A0;
  }
  else {
    candidate_40bc();
    iVar1 = extraout_A0_00;
  }
  *(undefined2 *)(iVar1 + 0x6e) = *(undefined2 *)(unaff_A6 + 0x6e);
  *(undefined2 *)(iVar1 + 0x72) = *(undefined2 *)(unaff_A6 + 0x72);
  *(undefined1 *)(iVar1 + 0x8c) = *(undefined1 *)(unaff_A6 + 0x8c);
  *(int *)(unaff_A6 + 0x82) = iVar1;
  candidate_1acf2();
  if (*(short *)(unaff_A6 + 0x74) != 0) {
    *(short *)(unaff_A6 + 0x7a) = *(short *)(unaff_A6 + 0x7a) + 1;
    if ((*(short *)(unaff_A6 + 0x74) != -1) &&
       (*(short *)(unaff_A6 + 0x74) <= *(short *)(unaff_A6 + 0x7a))) {
      *(undefined2 *)(unaff_A6 + 0x7a) = *(undefined2 *)(unaff_A6 + 0x74);
      *(byte *)(unaff_A6 + 0x8c) = *(byte *)(unaff_A6 + 0x8c) | 0x80;
    }
  }
  return;
}
