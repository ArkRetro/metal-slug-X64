/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06cb76.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6cb76(void)

{
  undefined2 uVar1;
  int extraout_A0;
  int unaff_A6;
  bool bVar2;

  uVar1 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x7c) = uVar1;
  uVar1 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x7e) = uVar1;
  uVar1 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x80) = uVar1;
  uVar1 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x82) = uVar1;
  candidate_1b04a();
  candidate_2eb4();
  candidate_260c8();
  candidate_2e7c();
  bVar2 = false;
  candidate_800c6();
  if (bVar2) {
    *(undefined2 *)(extraout_A0 + 0x6e) = *(undefined2 *)(unaff_A6 + 0x7c);
    *(undefined2 *)(extraout_A0 + 0x70) = *(undefined2 *)(unaff_A6 + 0x7e);
    *(undefined2 *)(extraout_A0 + 0x72) = *(undefined2 *)(unaff_A6 + 0x80);
    *(undefined2 *)(extraout_A0 + 0x74) = *(undefined2 *)(unaff_A6 + 0x82);
    *(undefined2 *)(extraout_A0 + 0x66) = 0x1110;
    *(undefined2 *)(extraout_A0 + 0x1e) = 0x1110;
  }
  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    candidate_1b220();
  }
  return;
}
