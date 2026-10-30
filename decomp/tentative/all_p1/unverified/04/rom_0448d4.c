/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0448d4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_448d4(void)

{
  undefined2 in_D0w;
  undefined2 in_D1w;
  int extraout_A0;
  int unaff_A6;
  bool bVar1;
  undefined2 uStack_e;
  undefined2 uStack_a;

  bVar1 = false;
  candidate_40bc();
  if (bVar1) {
    *(undefined4 *)(extraout_A0 + 0x6e) = *(undefined4 *)(unaff_A6 + 0x6e);
    *(undefined4 *)(extraout_A0 + 0x72) = *(undefined4 *)(unaff_A6 + 0x72);
    *(undefined1 *)(extraout_A0 + 0x80) = *(undefined1 *)(unaff_A6 + 0x80);
  }
  *(undefined2 *)(extraout_A0 + 0x86) = in_D0w;
  *(undefined2 *)(extraout_A0 + 0x88) = uStack_e;
  *(undefined2 *)(extraout_A0 + 0x8a) = in_D1w;
  *(undefined2 *)(extraout_A0 + 0x8c) = uStack_a;
  *(undefined1 *)(unaff_A6 + 0x60) = 0;
  return;
}
