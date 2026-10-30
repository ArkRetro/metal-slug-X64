/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07dd00.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_7dd00(void)

{
  undefined2 uVar1;
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_1abc8();
  if (_DAT_0010e99e <= *(short *)((int)unaff_A6 + 0x7e)) {
    if (*(char *)((int)unaff_A6 + 0x61) == '\0') {
      if (*(short *)((int)unaff_A6 + 0x6e) == 0) goto LAB_0007ddac;
      *(short *)(unaff_A6 + 0x1e) = *(short *)(unaff_A6 + 0x1e) + -1;
      if (*(short *)(unaff_A6 + 0x1e) < 1) {
        uVar1 = candidate_260c8();
        *(undefined2 *)(unaff_A6 + 0x1e) = uVar1;
        candidate_40bc();
        candidate_1acf2();
        *(undefined2 *)(extraout_A0 + 0x72) = *(undefined2 *)((int)unaff_A6 + 0x72);
        *(undefined2 *)(extraout_A0 + 0x74) = *(undefined2 *)(unaff_A6 + 0x1d);
        *(undefined2 *)(extraout_A0 + 0x76) = *(undefined2 *)((int)unaff_A6 + 0x76);
        if (*(short *)((int)unaff_A6 + 0x6e) != -1) {
          *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
        }
      }
    }
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    if (*(short *)(unaff_A6 + 0xf) < 0x11) {
      *unaff_A6 = candidate_7ddb4;
    }
    bVar2 = false;
    candidate_1945c();
    if (bVar2) {
      *unaff_A6 = candidate_7ddb4;
    }
    return;
  }
LAB_0007ddac:
  *unaff_A6 = candidate_7ddb4;
  return;
}
