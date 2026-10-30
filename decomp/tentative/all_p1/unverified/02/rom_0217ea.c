/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0217ea.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_217ea(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_4c1c();
  *(undefined2 *)(unaff_A6 + 0x1d) = 6;
  unaff_A6[0x14] = 0x21a0a;
  candidate_4256();
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  *unaff_A6 = 0x21834;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  bVar2 = false;
  if (*(short *)(unaff_A6 + 0x1d) != 0) {
    candidate_4c24();
    if ((bVar2) &&
       (*(undefined1 *)((int)unaff_A6 + 0x5a) = 0, (*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) {
      *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
      bVar2 = false;
      candidate_40bc();
      iVar1 = extraout_A0;
      if (bVar2) {
        candidate_1b220();
        iVar1 = extraout_A0_00;
      }
      if ((*(byte *)((int)unaff_A6 + 0x5b) & 0x80) == 0) {
        *(byte *)(iVar1 + 0x15) = *(byte *)(iVar1 + 0x15) & 0xfe;
      }
      else {
        *(byte *)(iVar1 + 0x15) = *(byte *)(iVar1 + 0x15) | 1;
      }
      *unaff_A6 = candidate_21812;
      if (*(short *)(unaff_A6 + 0x1d) == 0) {
        candidate_219cc();
      }
    }
  }
  candidate_219b0();
  return;
}
